pub fn mkclient() -> reqwest::Client {
    reqwest::Client::new()
}

pub fn post<T>(client: &reqwest::Client, url: &str, json: &T) -> reqwest::Result<reqwest::Response>
where
    T: serde::Serialize,
{
    client.post(url).json(json).send()
}

pub fn get<T>(client: &reqwest::Client, url: &str) -> reqwest::Result<T>
where
    T: serde::de::DeserializeOwned,
{
    client.get(url).send()?.json()
}

pub fn getstr(client: &reqwest::Client, url: &str) -> reqwest::Result<String> {
    client.get(url).send()?.text()
}

// pub fn postfile<T>(client: &reqwest::Client, url: &str, file: &std::fs::File) -> reqwest::Result<T> {
//     client.post(url).body(file).send()?.json()
// }
