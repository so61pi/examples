use rand::Rng;

pub mod config;
pub mod http;
pub mod kafka;
pub mod log;
pub mod message;
pub mod redis;

pub fn randstr(n: usize) -> String {
    let mut rng = rand::thread_rng();
    std::iter::repeat(())
        .map(|_| rng.sample(rand::distributions::Alphanumeric))
        .take(n)
        .collect()
}
