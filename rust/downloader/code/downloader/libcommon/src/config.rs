use clap::{value_t, ArgMatches, ErrorKind::ArgumentNotFound};
use config::{Config, Environment, File};
use serde::de::DeserializeOwned;

pub fn load<T>() -> T
where
    T: DeserializeOwned,
{
    let mut s = Config::new();
    match s
        // Read default config.
        .merge(File::with_name("config/default").required(false))
        .and_then(|s| {
            // Add in the current environment file, default to 'development' env.
            let env = std::env::var("RUNMODE").unwrap_or_else(|_| "development".into());
            s.merge(File::with_name(&format!("config/{}", env)).required(false))
        })
        .and_then(
            // Add in settings from the environment with a prefix of APP.
            |s| s.merge(Environment::with_prefix("app")),
        )
        // As `try_into` receives value of `s` (use `self` instead of `&mut self`),
        // we cannot write `.and_then(|s| s.try_into())` since the type of `s` is `&mut`.
        // Hence, converting `Result<&mut, E>` to `Result<(), E>` is necessary to
        // get rid of the reference (or also known as borrowed content).
        .map(|_| ())
        .and_then(|_| s.try_into())
    {
        Err(e) => {
            eprintln!("{}", e);
            std::process::exit(1);
        }
        Ok(v) => v,
    }
}

pub fn get_string(matches: &ArgMatches, name: &str, defval: String) -> String {
    matches
        .value_of(name)
        .map(|v| v.to_string())
        .unwrap_or(defval)
}

pub fn get_value<T>(matches: &ArgMatches, name: &str, defval: T) -> T
where
    T: std::str::FromStr,
{
    match value_t!(matches, name, T) {
        Ok(v) => v,
        Err(ref e) if e.kind == ArgumentNotFound => defval,
        Err(ref e) => e.exit(),
    }
}
