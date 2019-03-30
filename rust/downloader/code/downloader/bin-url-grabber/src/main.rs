use actix::Actor;
use chrono::Local;
use futures::Future;
use libcommon::kafka::HandlerError;
use libcommon::kafka::HandlerResult;
use libcommon::message::DownloadEnd;
use libcommon::message::DownloadResult;
use libcommon::message::DownloadStart;
use libcommon::message::HtmlDownloadResult;
use libcommon::message::UrlDownloadRequest;
use libcommon::redis::get_cached_value;
use libcommon::redis::set_cached_value;
use rand::distributions::Distribution;
use rand::distributions::Uniform;
use rand::rngs;
use rdkafka::producer::DeliveryFuture;
use std::collections::HashMap;
use std::default::Default;
use std::sync::Arc;

mod config;
mod grabber;

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
        config.kafka_topic_download_url_html.clone(),
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
            &[&config.kafka_topic_download_url_html],
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
            move |_| -> Box<dyn Future<Item = HtmlDownloadResult, Error = HandlerError>> {
                let value: Option<HtmlDownloadResult> = get_cached_value(
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
                        move |_| -> HandlerResult<HtmlDownloadResult> {
                            // Err(HandlerError::redo(failure::err_msg("test")))
                            let urls = grabber::geturls(
                                &client,
                                &url::Url::parse(&url.url).map_err(HandlerError::stop)?,
                            )
                            .map_err(HandlerError::stop)?;
                            urls.discarded.iter().for_each(|u| {
                                if u.len() > 40 {
                                    log::info!("discard '{:.len$}...'", u, len = 40);
                                } else {
                                    log::info!("discard '{}'", u);
                                }
                            });
                            Ok(HtmlDownloadResult {
                                htmlurls: urls
                                    .html
                                    .into_iter()
                                    .map(|e| url::Url::into_string(e.url))
                                    .collect(),
                                mediaurls: urls
                                    .media
                                    .into_iter()
                                    .map(|e| url::Url::into_string(e.url))
                                    .collect(),
                            })
                        }
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
                    ;

                Box::new(fut)
            }
        })
        .and_then(move |v| -> HandlerResult<DeliveryFuture> {
            serde_json::to_string(&DownloadEnd {
                urljobid: url.urljobid,
                dstoptime: Local::now(),
                comment: String::new(),
                result: DownloadResult::Html(v),
            })
            .map(|v| {
                libcommon::kafka::produce(
                    &producer,
                    &config.kafka_topic_download_notify_stop,
                    &key,
                    &v,
                )
            })
            .map_err(HandlerError::redo)
        })
        .and_then(|delivery| delivery.map_err(HandlerError::redo))
        .map(|_| ());

    Box::new(fut)
}
