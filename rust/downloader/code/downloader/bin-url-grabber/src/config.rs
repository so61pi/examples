use clap::{App, Arg};
use libcommon::config::{get_string, get_value};
use serde::Deserialize;

#[derive(Clone, Debug, Default, Deserialize)]
#[serde(default)]
pub struct Config {
    pub gate_addr: String,

    pub redis_addr: String,
    pub redis_prefix: String,
    pub redis_cache_minutes: u16,

    pub kafka_addr: String,
    pub kafka_group: String,
    pub kafka_topic_download_url_html: String,
    pub kafka_topic_download_notify_start: String,
    pub kafka_topic_download_notify_stop: String,

    pub download_delay_min_secs: u16,
    pub download_delay_max_secs: u16,

    pub log_level: libcommon::log::LogLevel,
}

pub fn cmdopt() -> Config {
    let mut config: Config = libcommon::config::load();
    let matches = App::new(env!("CARGO_PKG_NAME"))
        .about(env!("CARGO_PKG_DESCRIPTION"))
        .version(env!("CARGO_PKG_VERSION"))
        .author(env!("CARGO_PKG_AUTHORS"))
        .arg(
            Arg::with_name("gate-addr")
                .long("gate-addr")
                .value_name("ADDR")
                .takes_value(true)
                .required(config.gate_addr.is_empty())
                .help("gate address"),
        )
        .arg(
            Arg::with_name("redis-addr")
                .long("redis-addr")
                .value_name("ADDR")
                .takes_value(true)
                .required(config.redis_addr.is_empty())
                .help("redis address"),
        )
        .arg(
            Arg::with_name("redis-prefix")
                .long("redis-prefix")
                .value_name("PREFIX")
                .takes_value(true)
                .required(config.redis_prefix.is_empty())
                .help("redis key prefix"),
        )
        .arg(
            Arg::with_name("redis-cache-minutes")
                .long("redis-cache-minutes")
                .value_name("MINUTES")
                .takes_value(true)
                .required(config.redis_cache_minutes == 0)
                .help("redis cache time in minutes"),
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
                .help("kafka consumer group"),
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
            Arg::with_name("download-delay-min-secs")
                .long("download-delay-min-secs")
                .value_name("SECS")
                .takes_value(true)
                .required(config.download_delay_min_secs == 0)
                .help("min download delay time in seconds"),
        )
        .arg(
            Arg::with_name("download-delay-max-secs")
                .long("download-delay-max-secs")
                .value_name("SECS")
                .takes_value(true)
                .required(config.download_delay_max_secs == 0)
                .help("max download delay time in seconds"),
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

    config.gate_addr = get_string(&matches, "gate-addr", config.gate_addr);

    config.redis_addr = get_string(&matches, "redis-addr", config.redis_addr);
    config.redis_prefix = get_string(&matches, "redis-prefix", config.redis_prefix);
    config.redis_cache_minutes =
        get_value(&matches, "redis-cache-minutes", config.redis_cache_minutes);

    // Kafka options
    config.kafka_addr = get_string(&matches, "kafka-addr", config.kafka_addr);
    config.kafka_group = get_string(&matches, "kafka-group", config.kafka_group);

    config.kafka_topic_download_url_html = get_string(
        &matches,
        "kafka-topic-download-url-html",
        config.kafka_topic_download_url_html,
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

    config.download_delay_min_secs = get_value(
        &matches,
        "download-delay-min-secs",
        config.download_delay_min_secs,
    );
    config.download_delay_max_secs = get_value(
        &matches,
        "download-delay-max-secs",
        config.download_delay_max_secs,
    );
    if config.download_delay_min_secs > config.download_delay_max_secs {
        std::mem::swap(
            &mut config.download_delay_min_secs,
            &mut config.download_delay_max_secs,
        );
    }
    config.log_level = get_value(&matches, "log-level", config.log_level);

    config
}
