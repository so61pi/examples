use clap::{App, Arg};
use libcommon::config::get_string;
use libcommon::config::get_value;
use serde::Deserialize;

#[derive(Debug, Default, Deserialize)]
#[serde(default)]
pub struct Config {
    pub port: u16,

    pub postgresql_addr: String,

    pub start_rooturl: bool,
    pub start_urljob: bool,

    pub kafka_addr: String,
    pub kafka_group: String,
    pub kafka_topic_download_url_html: String,
    pub kafka_topic_download_url_media: String,
    pub kafka_topic_download_notify_start: String,
    pub kafka_topic_download_notify_stop: String,

    pub log_level: libcommon::log::LogLevel,
}

pub fn cmdopt() -> Config {
    let mut config: Config = libcommon::config::load();
    let matches = App::new(env!("CARGO_PKG_NAME"))
        .about(env!("CARGO_PKG_DESCRIPTION"))
        .version(env!("CARGO_PKG_VERSION"))
        .author(env!("CARGO_PKG_AUTHORS"))
        .arg(
            Arg::with_name("port")
                .long("port")
                .value_name("PORT")
                .takes_value(true)
                .required(config.port == 0)
                .help("port that gate listens on"),
        )
        .arg(
            Arg::with_name("postgresql-addr")
                .long("postgresql-addr")
                .value_name("ADDR")
                .takes_value(true)
                .required(config.postgresql_addr.is_empty())
                .help("postgresql connection string"),
        )
        .arg(
            Arg::with_name("start-urljob")
                .long("start-urljob")
                .value_name("BOOL")
                .takes_value(true)
                .help("start pending URL jobs"),
        )
        .arg(
            Arg::with_name("start-rooturl")
                .long("start-rooturl")
                .value_name("BOOL")
                .takes_value(true)
                .help("start root URLs on startup"),
        )
        .arg(
            Arg::with_name("kafka-addr")
                .long("kafka-addr")
                .value_name("ADDR")
                .takes_value(true)
                .required(config.kafka_addr.is_empty())
                .help("kafka address"),
        )
        .arg(
            Arg::with_name("kafka-group")
                .long("kafka-group")
                .value_name("GROUP")
                .takes_value(true)
                .required(config.kafka_group.is_empty())
                .help("kafka groupess"),
        )
        .arg(
            Arg::with_name("kafka-topic-download-url-html")
                .long("kafka-topic-download-url-html")
                .value_name("TOPIC")
                .takes_value(true)
                .required(config.kafka_topic_download_url_html.is_empty())
                .help("kafka topic download-url-html"),
        )
        .arg(
            Arg::with_name("kafka-topic-download-url-media")
                .long("kafka-topic-download-url-media")
                .value_name("TOPIC")
                .takes_value(true)
                .required(config.kafka_topic_download_url_media.is_empty())
                .help("kafka topic download-url-media"),
        )
        .arg(
            Arg::with_name("kafka-topic-download-notify-start")
                .long("kafka-topic-download-notify-start")
                .value_name("TOPIC")
                .takes_value(true)
                .required(config.kafka_topic_download_notify_start.is_empty())
                .help("kafka topic download-notify-start"),
        )
        .arg(
            Arg::with_name("kafka-topic-download-notify-stop")
                .long("kafka-topic-download-notify-stop")
                .value_name("TOPIC")
                .takes_value(true)
                .required(config.kafka_topic_download_notify_stop.is_empty())
                .help("kafka topic download-notify-stop"),
        )
        .arg(
            Arg::with_name("log-level")
                .long("log-level")
                .value_name("LEVEL")
                .takes_value(true)
                .possible_values(libcommon::log::LEVELS)
                .help("log level"),
        )
        .get_matches();

    config.port = get_value(&matches, "port", config.port);
    config.postgresql_addr = get_string(&matches, "postgresql-addr", config.postgresql_addr);

    config.start_rooturl = get_value(&matches, "start-rooturl", config.start_rooturl);
    config.start_urljob = get_value(&matches, "start-urljob", config.start_urljob);

    config.kafka_addr = get_string(&matches, "kafka-addr", config.kafka_addr);
    config.kafka_group = get_string(&matches, "kafka-group", config.kafka_group);

    config.kafka_topic_download_url_html = get_string(
        &matches,
        "kafka-topic-download-url-html",
        config.kafka_topic_download_url_html,
    );
    config.kafka_topic_download_url_media = get_string(
        &matches,
        "kafka-topic-download-url-media",
        config.kafka_topic_download_url_media,
    );
    config.kafka_topic_download_notify_start = get_string(
        &matches,
        "kafka-topic-download-notify-start",
        config.kafka_topic_download_notify_start,
    );
    config.kafka_topic_download_notify_stop = get_string(
        &matches,
        "kafka-topic-download-notify-stop",
        config.kafka_topic_download_notify_stop,
    );

    config.log_level = get_value(&matches, "log-level", config.log_level);

    config
}
