use redis::Commands;

pub fn get_cached_value<T>(addr: &str, url: &str) -> Option<T>
where
    T: serde::de::DeserializeOwned,
{
    let s: Option<String> = redisconn(addr)
        .map_err(|e| log::warn!("connect to redis failed with error '{}'", e))
        .map(|c| c.get(url).ok())
        .unwrap_or(None);

    if let Some(_v) = &s {
        log::info!("cache hit for '{}'", url);
    }

    s.map(|s| serde_json::from_str(&s).ok()).unwrap_or(None)
}

pub fn set_cached_value<T>(addr: &str, url: &str, data: &T, secs: usize)
where
    T: serde::Serialize,
{
    let _ = serde_json::to_string(data).map(|s| {
        redisconn(addr)
            .map(|c| {
                c.set(url, &s)
                    .and_then(|_: ()| c.expire(url, secs))
                    .and_then(|_: ()| Ok(()))
            })
            .map_err(|e| log::warn!("connect to redis failed with error '{}'", e))
    });
}

fn redisconn(addr: &str) -> redis::RedisResult<redis::Connection> {
    redis::Client::open(addr)?.get_connection()
}
