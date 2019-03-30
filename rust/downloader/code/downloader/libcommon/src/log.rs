use serde::Deserialize;

#[derive(Clone, Debug, Deserialize)]
pub enum LogLevel {
    Off,
    Error,
    Warn,
    Info,
    Debug,
    Trace,
}

pub const DEFLEVEL: LogLevel = LogLevel::Info;
pub const LEVELS: &[&str] = &["off", "error", "warn", "info", "debug", "trace"];

#[derive(Debug)]
pub struct ParseLogLevelErrror(pub String);

impl std::fmt::Display for ParseLogLevelErrror {
    fn fmt(&self, f: &mut std::fmt::Formatter) -> std::fmt::Result {
        write!(f, "{} is an invalid instance of LogLevel", self.0)
    }
}

impl std::error::Error for ParseLogLevelErrror {}

impl Default for LogLevel {
    fn default() -> Self {
        DEFLEVEL
    }
}

impl std::str::FromStr for LogLevel {
    type Err = ParseLogLevelErrror;

    fn from_str(s: &str) -> Result<Self, Self::Err> {
        match s {
            "off" => Ok(LogLevel::Off),
            "error" => Ok(LogLevel::Error),
            "warn" => Ok(LogLevel::Warn),
            "info" => Ok(LogLevel::Info),
            "debug" => Ok(LogLevel::Debug),
            "trace" => Ok(LogLevel::Trace),
            _ => Err(ParseLogLevelErrror(s.to_string())),
        }
    }
}

impl std::convert::From<LogLevel> for simplelog::LevelFilter {
    fn from(level: LogLevel) -> Self {
        match level {
            LogLevel::Off => simplelog::LevelFilter::Off,
            LogLevel::Error => simplelog::LevelFilter::Error,
            LogLevel::Warn => simplelog::LevelFilter::Warn,
            LogLevel::Info => simplelog::LevelFilter::Info,
            LogLevel::Debug => simplelog::LevelFilter::Debug,
            LogLevel::Trace => simplelog::LevelFilter::Trace,
        }
    }
}
