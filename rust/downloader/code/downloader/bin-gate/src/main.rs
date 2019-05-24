use actix_web::actix::Actor;
use actix_web::http;
use actix_web::server;
use actix_web::App;
use actix_web::HttpRequest as OriginalHttpRequest;
use actix_web::HttpResponse;
use actix_web::Json;
use chrono::Local;
use futures::future::Future;
use futures::future::IntoFuture;
use libcommon::kafka::HandlerError;
use libcommon::kafka::HandlerResult;
use libcommon::message::DownloadEnd;
use libcommon::message::DownloadResult;
use libcommon::message::DownloadStart;
use libcommon::message::EndType;
use libcommon::message::HtmlDownloadResult;
use libcommon::message::MediaDownloadResult;
use libcommon::message::UrlDownloadRequest;
use libcommon::message::UrlType;
use libcommon::message::WorkspaceCreationRequest;
use libcommon::message::WorkspaceCreationResponse;
use libcommon::message::WorkspaceDeletionRequest;
use rdkafka::producer::DeliveryFuture;
use regex::Regex;
use regex::RegexSet;
use serde::Deserialize;
use serde::Serialize;
use std::sync::Arc;
use std::sync::Mutex;

mod config;
mod error;
mod state;

const RESERVED_URLJOB_ID: i64 = 1;

type HttpRequest = OriginalHttpRequest<Arc<state::State>>;
type HttpResult<T> = std::result::Result<T, error::Error>;

fn wraphttp<T>(r: HttpResult<T>) -> HttpResponse
where
    T: Serialize,
{
    match r {
        Err(e) => {
            log::error!("error {}", e);
            HttpResponse::InternalServerError().finish()
        }
        Ok(v) => HttpResponse::Ok().json(v),
    }
}

fn wrapkafka<T>(v: T) -> Box<dyn Future<Item = (), Error = HandlerError>>
where
    T: IntoFuture<Item = (), Error = HandlerError> + 'static,
{
    Box::new(v.into_future())
}

/*
{
    "htmlfilter": [
        {
            "pattern": "asdasdasd",
            "include": [],
            "exclude": [],
            "maxurls": 50
        }
    ]
}
*/

#[derive(Debug, Default, Deserialize)]
#[serde(default)]
struct UrlFilterRule {
    pattern: String,
    include: Vec<String>,
    exclude: Vec<String>,
    maxurls: Option<u16>,
}

#[derive(Debug, Default, Deserialize)]
#[serde(default)]
struct WorkspaceConfig {
    htmlfilter: Vec<UrlFilterRule>,
}

fn get_workspace_config(
    dbconn: &postgres::Connection,
    workspaceid: i64,
) -> failure::Fallible<WorkspaceConfig> {
    let values: &[&postgres::types::ToSql] = &[
        &workspaceid, // $1
    ];

    let r = dbconn.query(
        "SELECT config
        FROM workspace
        WHERE id = $1;",
        values,
    )?;

    let json: Option<serde_json::Value> = r.iter().next().map(|row| row.get(0));

    if json.is_none() {
        failure::bail!("missing config for workspace {}", workspaceid);
    }

    serde_json::from_value(json.unwrap()).map_err(failure::Error::from)
}

#[allow(dead_code)]
fn get_workspace_config_by_urljobid(
    dbconn: &postgres::Connection,
    urljobid: i64,
) -> failure::Fallible<WorkspaceConfig> {
    let values: &[&postgres::types::ToSql] = &[
        &urljobid, // $1
    ];

    let r = dbconn.query(
        "SELECT config
        FROM workspace INNER JOIN urljob
            ON workspace.id = urljob.workspaceid
        WHERE urljob.id = $1;",
        values,
    )?;

    let json: Option<serde_json::Value> = r.iter().next().map(|row| row.get(0));

    if json.is_none() {
        failure::bail!(
            "missing config for workspace derived from urljob {}",
            urljobid
        );
    }

    serde_json::from_value(json.unwrap()).map_err(failure::Error::from)
}

fn create_workspace(
    (item, req): (Json<WorkspaceCreationRequest>, HttpRequest),
) -> HttpResult<WorkspaceCreationResponse> {
    let id: i64 = req
        .state()
        .dbconn()
        .query(
            "INSERT INTO workspace (name) VALUES ($1) RETURNING id",
            &[&item.name],
        )
        .map(|result| {
            result
                .iter()
                .next()
                .map(|id| id.get(0))
                .expect("urljob.workspaceid is not returned")
        })?;
    Ok(WorkspaceCreationResponse { id: id })
}

fn delete_workspace((item, req): (Json<WorkspaceDeletionRequest>, HttpRequest)) -> HttpResult<()> {
    req.state()
        .dbconn()
        .execute("DELETE FROM workspace WHERE name = $1", &[&item.name])?;
    Ok(())
}

#[derive(Debug)]
struct RootUrl {
    workspaceid: i64,
    url: String,
    period: i32, /* >0 */
}

fn startjob(state: Arc<state::State>) -> Box<dyn Future<Item = Vec<i64>, Error = failure::Error>> {
    let r = state
        .dbconn()
        .query("SELECT id FROM urljob ORDER BY id DESC LIMIT 1;", &[]);
    let r = match r.map_err(failure::Error::from) {
        Err(e) => return Box::new(futures::future::err(e)),
        Ok(v) => v,
    };

    let lastid: i64 = match r.iter().next().map(|row| row.get(0)) {
        None => return Box::new(futures::future::ok(vec![])),
        Some(v) => v,
    };
    let lastid = lastid + 1;

    let r = state.dbconn().query(
        "SELECT id, url, urltype
        FROM urljob
        WHERE id < $1 AND begintime IS NULL;",
        &[&lastid],
    );
    let r = match r.map_err(failure::Error::from) {
        Err(e) => return Box::new(futures::future::err(e)),
        Ok(v) => v,
    };

    let vec = r
        .iter()
        .map(|row| (row.get(0), row.get(1), row.get(2)))
        .map(|(urljobid, url, urltype): (i64, String, UrlType)| {
            let topic = match urltype {
                UrlType::Media => state.config.kafka_topic_download_url_media.as_str(),
                UrlType::Html => state.config.kafka_topic_download_url_html.as_str(),
            };
            push_urljob_to_kafka(
                &state.producer(),
                topic,
                &UrlDownloadRequest {
                    urljobid: urljobid,
                    url: url,
                },
            )
            .map_err(failure::Error::from)
            .into_future()
            .and_then(|delivery| delivery.map_err(failure::Error::from))
            .and_then(|result| result.map_err(|(e, _m)| failure::Error::from(e)))
            .and_then({
                let state = state.clone();
                move |(_partition, _offset)| {
                    update_urljob_begintime(&state.dbconn(), urljobid).map_err(failure::Error::from)
                }
            })
            .map(move |_| urljobid)
        })
        .map(|_v| _v)
        .collect::<Vec<_>>();

    Box::new(futures::future::join_all(vec))
}

fn startroot(
    state: Arc<state::State>,
) -> Box<dyn Future<Item = Vec<(i64, String)>, Error = HandlerError>> {
    let r = match state
        .dbconn()
        .query("SELECT workspaceid, url, period FROM rooturl", &[])
        .map_err(HandlerError::stop)
    {
        Err(e) => return Box::new(futures::future::err(e)),
        Ok(v) => v,
    };

    let vec = r
        .iter()
        .map(move |row| RootUrl {
            workspaceid: row.get(0),
            url: row.get(1),
            period: row.get(2),
        })
        .map(move |rooturl| {
            log::info!("{:?}", rooturl);
            mkurljob(
                state.clone(),
                RESERVED_URLJOB_ID,
                rooturl.workspaceid,
                rooturl.url.clone(),
                UrlType::Html,
            )
            .map(|_| (rooturl.workspaceid, rooturl.url))
        })
        .collect::<Vec<_>>();
    Box::new(futures::future::join_all(vec))
}

fn kafka_topic_download_notify_start(state: &state::State, message: &str) -> HandlerResult<()> {
    let item: DownloadStart = serde_json::from_str(message).map_err(HandlerError::stop)?;

    let values: &[&postgres::types::ToSql] = &[
        &item.urljobid,   // $1
        &item.dstarttime, // $2
    ];
    state
        .dbconn()
        .execute(
            "UPDATE urljob
            SET dstarttime = $2
            WHERE id = $1 AND dstarttime IS NULL;",
            values,
        )
        .map(|_| ())
        .map_err(HandlerError::redo)
}

fn insert_urljob_to_db(
    dbconn: &postgres::Connection,
    parentid: i64,
    workspaceid: i64,
    url: String,
    urltype: UrlType,
) -> HandlerResult<i64> {
    let values: &[&postgres::types::ToSql] = &[
        &parentid,     // $1
        &workspaceid,  // $2
        &url,          // $3
        &urltype,      // $4
        &Local::now(), // $5
    ];
    let r = dbconn.query(
        "WITH ta AS (
            INSERT INTO urljob (parentid, workspaceid, url, urltype, inserttime)
                VALUES ($1, $2, $3, $4, $5)
            ON CONFLICT (parentid, workspaceid, md5(url), urltype) WHERE parentid <> 1
                DO UPDATE SET parentid = NULL WHERE FALSE
            RETURNING id AS urljobid
        )
        SELECT urljobid FROM ta
        UNION ALL
        SELECT id AS urljobid FROM urljob -- never executed if INSERT successful
            WHERE parentid = $1
            AND workspaceid = $2
            AND url = $3
            AND urltype = $4
        LIMIT 1;",
        values,
    );

    r.map_err(HandlerError::redo).map(|rows| {
        rows.iter()
            .next()
            .map(|row| row.get(0))
            .expect("urljob.id is not returned")
    })
}

fn push_urljob_to_kafka<T>(
    producer: &rdkafka::producer::FutureProducer,
    topic: &str,
    data: &T,
) -> Result<DeliveryFuture, serde_json::error::Error>
where
    T: serde::Serialize,
{
    serde_json::to_string(data).map(|jsonmsg| {
        libcommon::kafka::produce(producer, &topic, &libcommon::randstr(10), &jsonmsg)
    })
}

fn update_urljob_begintime(dbconn: &postgres::Connection, urljobid: i64) -> postgres::Result<u64> {
    let values: &[&postgres::types::ToSql] = &[
        &Local::now(), // $1
        &urljobid,     // $2
    ];

    // If id is missing, then maybe the job is somehow deleted,
    // so we can stop processing this message.
    // If it's there then, obviously stop processing this message.
    dbconn.execute(
        "UPDATE urljob
        SET begintime = $1
        WHERE id = $2 AND begintime IS NULL;",
        values,
    )
}

pub fn mkurljob(
    state: Arc<state::State>,
    parentid: i64,
    workspaceid: i64,
    url: String,
    urltype: UrlType,
) -> Box<dyn Future<Item = (), Error = HandlerError>> {
    let fut = futures::future::ok(())
        .and_then({
            let url = url.clone();
            let urltype = urltype.clone();
            let state = state.clone();
            move |_| insert_urljob_to_db(&state.dbconn(), parentid, workspaceid, url, urltype)
        })
        .and_then({
            let state = state.clone();
            move |urljobid| -> HandlerResult<(i64, Option<DeliveryFuture>)> {
                let topic = match urltype {
                    UrlType::Media => &state.config.kafka_topic_download_url_media,
                    UrlType::Html => &state.config.kafka_topic_download_url_html,
                };

                // If begintime is not null (which means we are re-processing
                // and the kafka message is already pushed succesfully), we
                // don't need to send kafka message again.
                let r = state
                    .dbconn()
                    .query(
                        "SELECT id
                        FROM urljob
                        WHERE id = $1 AND begintime IS NULL;",
                        &[&urljobid],
                    )
                    .map_err(HandlerError::redo)?;

                if r.iter().next().is_none() {
                    return Ok((urljobid, None));
                }

                push_urljob_to_kafka(
                    &state.producer(),
                    topic,
                    &UrlDownloadRequest {
                        urljobid: urljobid,
                        url: url,
                    },
                )
                .map(Some)
                .map(|v| (urljobid, v))
                .map_err(HandlerError::stop)
            }
        })
        .and_then(
            move |(urljobid, v)| -> Box<dyn Future<Item = i64, Error = HandlerError>> {
                match v {
                    None => Box::new(futures::future::ok(urljobid)),
                    Some(v) => Box::new(v.map_err(HandlerError::redo).map(move |_v| urljobid)),
                }
            },
        )
        .and_then(move |urljobid| {
            update_urljob_begintime(&state.dbconn(), urljobid)
                .map(|_| ())
                .map_err(HandlerError::redo)
        });
    Box::new(fut)
}

fn urlfilter<I, A>(wsconfig: &WorkspaceConfig, parenturl: &str, htmls: I) -> Vec<String>
where
    I: IntoIterator<Item = A>,
    A: AsRef<str>,
{
    // find first matching rule
    let rule = wsconfig
        .htmlfilter
        .iter()
        .find(|v| match Regex::new(&v.pattern) {
            Ok(r) => r.is_match(parenturl),
            Err(e) => {
                log::warn!("regex parsing error '{}'", e);
                false
            }
        });

    if let Some(rule) = rule {
        let mut filtered = vec![];

        // empty include is same as ['.*']
        let inset = if rule.include.is_empty() {
            RegexSet::new(
                rule.include
                    .iter()
                    .map(AsRef::as_ref)
                    .chain(std::iter::once(".*")),
            )
        } else {
            RegexSet::new(rule.include.iter().map(|e| e as &str))
        };

        let inset = match inset {
            Ok(v) => v,
            Err(e) => {
                log::warn!("regex parsing error '{}'", e);
                RegexSet::new(&[".*"]).expect("regex '.*' cannot fail")
            }
        };
        let exset = match RegexSet::new(&rule.exclude) {
            Ok(v) => v,
            Err(e) => {
                log::warn!("regex parsing error '{}'", e);
                RegexSet::new(&[] as &[&str]).expect("empty regex set cannot fail")
            }
        };

        let mut tmp = htmls
            .into_iter()
            .filter_map(|u| {
                // include filter with order
                inset.matches(u.as_ref()).iter().nth(0).map(|n| (n, u))
            })
            .filter_map(|(n, u)| {
                // exclude filter
                if exset.matches(u.as_ref()).len() > 0 {
                    None
                } else {
                    Some((n, u))
                }
            })
            .collect::<Vec<_>>();
        tmp.sort_by_key(|(n, _u)| *n);
        filtered.extend(
            tmp.iter()
                .take(rule.maxurls.unwrap_or(std::u16::MAX) as usize)
                .map(|(_n, u)| String::from(u.as_ref())),
        );
        filtered
    } else {
        Vec::new()
    }
}

fn process_media_result(
    state: Arc<state::State>,
    urljobid: i64,
    v: &MediaDownloadResult,
) -> Box<dyn Future<Item = (), Error = HandlerError>> {
    // TODO: Why this only works with &
    let values: &[&postgres::types::ToSql] = &[
        &urljobid,      // $1
        &v.filehash,    // $2
        &v.hashtype,    // $3
        &v.filesize,    // $4
        &v.filename,    // $5
        &v.mediawidth,  // $6
        &v.mediaheight, // $7
        &Local::now(),  // $8
    ];

    let r = state
        .dbconn()
        .query(
            // https://blog.jooq.org/2017/05/31/how-to-execute-a-sql-query-only-if-another-sql-query-has-no-results/
            "-- EXPLAIN ANALYZE
            WITH ta AS (
                INSERT INTO fileinfo(filehash, hashtype, filesize)
                VALUES($2, $3, $4)
                ON CONFLICT (filehash) DO UPDATE SET filesize = NULL WHERE FALSE
                RETURNING id AS fileinfoid
            )
            , tb AS (
                SELECT fileinfoid FROM ta
                UNION ALL
                SELECT id AS fileinfoid FROM fileinfo -- never executed if INSERT successful
                WHERE filehash = $2
                LIMIT 1
            )
            INSERT INTO mediafile(urljobid, fileinfoid, filename, mediawidth, mediaheight, inserttime)
            SELECT $1, tb.fileinfoid, $5, $6, $7, $8 FROM tb
            ON CONFLICT DO NOTHING;",
            values,
        )
        .map_err(HandlerError::redo)
        .map(|_|());

    Box::new(futures::future::result(r))
}

fn process_html_result(
    state: Arc<state::State>,
    urljobid: i64,
    workspaceid: i64,
    parenturl: &str,
    v: &HtmlDownloadResult,
) -> Box<dyn Future<Item = (), Error = HandlerError>> {
    // filter and sort HTML URLs
    let mut filtered = vec![];
    let wsconfig = get_workspace_config(&state.dbconn(), workspaceid);
    let htmlurls = match wsconfig {
        Err(e) => {
            log::warn!("{}", e);
            &v.htmlurls
        }
        Ok(wsconfig) => {
            let x = &v.htmlurls;
            filtered.extend(urlfilter(&wsconfig, parenturl, x));
            &filtered
        }
    };

    let removed = v.htmlurls.len() - filtered.len();
    log::info!(
        "receive {:>5} html urls ({:>5} removed) and {:>5} media urls from urljob {:>10}",
        v.htmlurls.len(),
        removed,
        v.mediaurls.len(),
        urljobid
    );

    // create new urljobs
    let a = v
        .mediaurls
        .iter()
        .map(move |url| (url.clone(), UrlType::Media));
    let b = htmlurls.iter().map(move |url| (url.clone(), UrlType::Html));
    let ab = a
        .chain(b)
        .collect::<Vec<_>>()
        .into_iter()
        .map(move |(url, urltype)| mkurljob(state.clone(), urljobid, workspaceid, url, urltype));

    Box::new(futures::future::join_all(ab).map(|_| ()))
}

fn kafka_topic_download_notify_stop(
    state: Arc<state::State>,
    message: &str,
) -> Box<dyn Future<Item = (), Error = HandlerError>> {
    let item: Arc<DownloadEnd> = match serde_json::from_str(message).map_err(HandlerError::stop) {
        Err(e) => return Box::new(futures::future::err(e)),
        Ok(v) => Arc::new(v),
    };

    let fut = futures::future::ok(())
        .and_then({
            let state = state.clone();
            let item = item.clone();
            move |_| -> HandlerResult<(i64, String)> {
                let values: &[&postgres::types::ToSql] = &[
                    &item.urljobid,  // $1
                    &item.dstoptime, // $2
                ];

                let r = state
                    .dbconn()
                    .query(
                        "WITH ta AS (
                            UPDATE urljob
                            SET dstoptime = $2
                            WHERE id = $1 AND dstoptime IS NULL
                            RETURNING workspaceid, url
                        )
                        SELECT workspaceid, url FROM ta
                        UNION ALL
                        SELECT workspaceid, url FROM urljob -- never executed if UPDATE successful
                        WHERE id = $1
                        LIMIT 1;",
                        values,
                    )
                    .map_err(HandlerError::redo);

                r.map(|rows| rows.iter().next().map(|row| (row.get(0), row.get(1))))
                    .transpose()
                    .ok_or_else(|| failure::err_msg(format!("urljob {} not found", item.urljobid)))
                    .map_err(HandlerError::stop)?
            }
        })
        .and_then({
            let state = state.clone();
            let item = item.clone();
            move |(workspaceid, parenturl)| -> Box<dyn Future<Item = (), Error = HandlerError>> {
                let urljobid = item.urljobid;
                match &item.result {
                    DownloadResult::Error => Box::new(futures::future::ok(())),
                    DownloadResult::Media(v) => {
                        process_media_result(state.clone(), urljobid, v)
                    }
                    DownloadResult::Html(v) => {
                        process_html_result(state.clone(), urljobid, workspaceid, &parenturl, v)
                    }
                }
            }
        })
        .and_then({
            let state = state.clone();
            let item = item.clone();
            move |_| -> HandlerResult<()> {
                let values: &[&postgres::types::ToSql] = &[
                    &item.urljobid,    // $1
                    &Local::now(),     // $2
                    &EndType::Success, // $3
                ];

                state
                    .dbconn()
                    .query(
                        "UPDATE urljob
                        SET endtime = $2, endtype = $3
                        WHERE id = $1 AND endtime IS NULL AND endtype IS NULL;",
                        values,
                    )
                    .map(|_| ())
                    .map_err(HandlerError::redo)
            }
        })
        .and_then(|_| futures::future::ok(()));

    Box::new(fut)
}

fn main() -> std::result::Result<(), Box<dyn std::error::Error>> {
    let config = config::cmdopt();
    simplelog::TermLogger::init(
        config.log_level.clone().into(),
        simplelog::Config::default(),
    )?;
    log::info!("{:?}", config);

    let producer = libcommon::kafka::mkproducer(&config.kafka_addr)?;
    let dbconn =
        postgres::Connection::connect(config.postgresql_addr.as_str(), postgres::TlsMode::None)?;

    let state = state::State::new(config, Mutex::new(dbconn), Mutex::new(producer));

    let sys = actix_web::actix::System::new("system");

    if state.config.start_rooturl {
        actix_web::actix::spawn(startroot(state.clone()).map(|_v| ()).map_err(|_v| ()));
    }

    if state.config.start_urljob {
        actix_web::actix::spawn(startjob(state.clone()).map(|_v| ()).map_err(|_v| ()));
    }

    setup_consumer(state.clone())?.start();

    // signals are already handled in actix code
    server::new({
        let state = state.clone();
        move || {
            App::with_state(state.clone())
                .route(
                    "/api/v1/create-workspace",
                    http::Method::POST,
                    |(item, req)| wraphttp(create_workspace((item, req))),
                )
                .route(
                    "/api/v1/delete-workspace",
                    http::Method::POST,
                    |(item, req)| wraphttp(delete_workspace((item, req))),
                )
        }
    })
    .bind(format!("0.0.0.0:{}", state.config.port))?
    .start();

    sys.run();

    Ok(())
}

fn setup_consumer(state: Arc<state::State>) -> failure::Fallible<libcommon::kafka::ConsumerActor> {
    let mut handlers = libcommon::kafka::HandlerMap::new();
    handlers.insert(
        state.config.kafka_topic_download_notify_start.clone(),
        Box::new({
            let state = state.clone();
            move |message| wrapkafka(kafka_topic_download_notify_start(&state, message))
        }),
    );
    handlers.insert(
        state.config.kafka_topic_download_notify_stop.clone(),
        Box::new({
            let state = state.clone();
            move |message| kafka_topic_download_notify_stop(state.clone(), message)
        }),
    );

    let consumer_actor = libcommon::kafka::ConsumerActor::new(
        libcommon::kafka::mkconsumer(
            &state.config.kafka_addr,
            &state.config.kafka_group,
            &[
                &state.config.kafka_topic_download_notify_start,
                &state.config.kafka_topic_download_notify_stop,
            ],
        )?,
        handlers,
    );

    Ok(consumer_actor)
}
