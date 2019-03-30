use actix::Actor;
use chrono::Local;
use futures::Future;
use libcommon::kafka::HandlerError;
use libcommon::kafka::HandlerResult;
use libcommon::message::DownloadEnd;
use libcommon::message::DownloadResult;
use libcommon::message::DownloadStart;
use libcommon::message::HashType;
use libcommon::message::MediaDownloadResult;
use libcommon::message::UrlDownloadRequest;
use libcommon::redis::get_cached_value;
use libcommon::redis::set_cached_value;
use rand::distributions::Distribution;
use rand::distributions::Uniform;
use rand::rngs;
use rdkafka::producer::DeliveryFuture;
use sha2::Digest;
use std::collections::HashMap;
use std::fs::File;
use std::io::copy;
use std::sync::Arc;

mod config;

fn main() -> Result<(), Box<dyn std::error::Error>> {
    let config = config::cmdopt();
    simplelog::TermLogger::init(
        config.log_level.clone().into(),
        simplelog::Config::default(),
    )?;
    log::info!("{:?}", config);

    let sys = actix::System::new("system");
    actix::actors::signal::DefaultSignalsHandler::start_default();
    setup_consumer(Arc::new(config))?.start();
    sys.run();

    Ok(())
}

fn setup_consumer(
    config: Arc<config::Config>,
) -> failure::Fallible<libcommon::kafka::ConsumerActor> {
    let mut handlers: libcommon::kafka::HandlerMap = HashMap::new();
    handlers.insert(
        config.kafka_topic_download_url_media.clone(),
        Box::new({
            let config = config.clone();
            let client = Arc::new(libcommon::http::mkclient());
            let producer = Arc::new(libcommon::kafka::mkproducer(&config.kafka_addr)?);
            move |message| handle_message(config.clone(), client.clone(), producer.clone(), message)
        }),
    );

    Ok(libcommon::kafka::ConsumerActor::new(
        libcommon::kafka::mkconsumer(
            &config.kafka_addr,
            &config.kafka_group,
            &[&config.kafka_topic_download_url_media],
        )?,
        handlers,
    ))
}

fn handle_message(
    config: Arc<config::Config>,
    client: Arc<reqwest::Client>,
    producer: Arc<rdkafka::producer::FutureProducer>,
    message: &str,
) -> Box<dyn Future<Item = (), Error = HandlerError>> {
    let url: Arc<UrlDownloadRequest> = match serde_json::from_str(message) {
        Err(e) => return Box::new(futures::future::err(HandlerError::stop(e))),
        Ok(v) => Arc::new(v),
    };

    let key = libcommon::randstr(10);

    let fut = futures::future::ok(())
        .and_then({
            let config = config.clone();
            let url = url.clone();
            let producer = producer.clone();
            let key = key.clone();
            move |_| -> HandlerResult<DeliveryFuture> {
                serde_json::to_string(&DownloadStart {
                    urljobid: url.urljobid,
                    dstarttime: Local::now(),
                })
                .map(|v| {
                    libcommon::kafka::produce(
                        &producer,
                        &config.kafka_topic_download_notify_start,
                        &key,
                        &v,
                    )
                })
                .map_err(HandlerError::stop)
            }
        })
        .and_then(|delivery| delivery.map_err(HandlerError::redo))
        .and_then({
            let url = url.clone();
            let config = config.clone();
            move |_| -> Box<dyn Future<Item = MediaDownloadResult, Error = HandlerError>> {
                let value: Option<MediaDownloadResult> = get_cached_value(
                    &config.redis_addr,
                    &format!("{}{}", config.redis_prefix, url.url),
                );

                if let Some(v) = value {
                    return Box::new(futures::future::ok(v));
                }

                let fut = futures::future::ok(())
                    .and_then({
                        let url = url.clone();
                        let config = config.clone();
                        move |_| {
                            let delayms = Uniform::new_inclusive(
                                u64::from(config.download_delay_min_secs) * 1000,
                                u64::from(config.download_delay_max_secs) * 1000,
                            )
                            .sample(&mut rngs::ThreadRng::default());
                            log::info!("delay {:>6}ms before downloading '{}'", delayms, url.url);
                            let time = std::time::Instant::now()
                                + std::time::Duration::from_millis(delayms);
                            tokio_timer::Delay::new(time).map_err(HandlerError::redo)
                        }
                    })
                    .and_then({
                        let url = url.clone();
                        let config = config.clone();
                        move |_| download_file_impl(&config, &client, &url)
                    })
                    .and_then(move |v| {
                        set_cached_value(
                            &config.redis_addr,
                            &format!("{}{}", config.redis_prefix, url.url),
                            &v,
                            usize::from(config.redis_cache_minutes) * 60,
                        );
                        Ok(v)
                    });

                Box::new(fut)
            }
        })
        .and_then({
            move |v| -> HandlerResult<DeliveryFuture> {
                let v = DownloadEnd {
                    urljobid: url.urljobid,
                    dstoptime: Local::now(),
                    comment: String::new(),
                    result: DownloadResult::Media(v),
                };
                serde_json::to_string(&v)
                    .map(|v| {
                        libcommon::kafka::produce(
                            &producer,
                            &config.kafka_topic_download_notify_stop,
                            &key,
                            &v,
                        )
                    })
                    .map_err(HandlerError::stop)
            }
        })
        .map(|_| ());

    Box::new(fut)
}

// TODO: Convert this function to async
fn download_file_impl(
    config: &config::Config,
    client: &reqwest::Client,
    url: &UrlDownloadRequest,
) -> HandlerResult<MediaDownloadResult> {
    let mut response = client.get(&url.url).send().map_err(HandlerError::stop)?;

    // let length = response
    //     .content_length()
    //     .ok_or_else(|| failure::err_msg("missing content length"))
    //     .map_err(HandlerError::stop)?;
    // if length > config.limit_filesize.into() {
    //     return Err(HandlerError::stop(failure::err_msg("file limit exceeded")));
    // }

    let content_type = response
        .headers()
        .get(reqwest::header::CONTENT_TYPE)
        .and_then(|v| v.to_str().ok())
        .ok_or_else(|| failure::err_msg("missing content-type"))
        .map_err(HandlerError::stop)?;

    if content_type.starts_with("text/html;") {
        return Err(HandlerError::stop(failure::err_msg(
            "TODO: media link is HTML",
        )));
    }

    let ext = match content_type {
        "image/png" => "png",
        "image/jpeg" => "jpg",
        "image/bmp" => "bmp",
        "image/gif" => "gif",
        "image/webp" => "webp",
        "video/webm" => "webm",
        "video/mp4" => "mp4",
        _ => {
            return Err(failure::format_err!(
                "cannot handle content-type '{}'",
                content_type
            ))
            .map_err(HandlerError::stop);
        }
    };

    let mut content: Vec<u8> = vec![];
    copy(&mut response, &mut content).map_err(HandlerError::stop)?;

    let mut hasher = sha2::Sha256::new();
    copy(&mut std::io::Cursor::new(&content), &mut hasher).map_err(HandlerError::stop)?;
    let filehash = format!("{:x}", hasher.result());

    let real_filename = format!("{}.{}", filehash, ext);
    let report_filename = response
        .url()
        .path_segments()
        .and_then(Iterator::last)
        .map(ToString::to_string)
        .map(|s| {
            if s.is_empty() {
                real_filename.clone()
            } else {
                s
            }
        })
        .unwrap();

    let (a, b) = real_filename.split_at(2);
    let dir = std::path::Path::new(&config.download_dir).join(a);
    let real_filename = dir.join(b);

    std::fs::create_dir_all(dir).map_err(HandlerError::redo)?;
    let mut dest = File::create(&real_filename).map_err(HandlerError::redo)?;
    copy(&mut std::io::Cursor::new(&content), &mut dest).map_err(HandlerError::redo)?;

    // TODO: Check media width and height
    Ok(MediaDownloadResult {
        filename: report_filename,
        filesize: content.len() as i64, /* TODO */
        filehash: filehash,
        hashtype: HashType::Sha2256,
        mediawidth: 1,  /* TODO */
        mediaheight: 1, /* TODO */
    })
}
