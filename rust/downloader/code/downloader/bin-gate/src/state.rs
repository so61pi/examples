use std::sync::Arc;
use std::sync::Mutex;
use std::sync::MutexGuard;

pub struct State {
    pub config: crate::config::Config,
    pub dbconn: Mutex<postgres::Connection>,
    pub producer: Mutex<rdkafka::producer::FutureProducer>,
}

impl State {
    pub fn new(
        config: crate::config::Config,
        dbconn: Mutex<postgres::Connection>,
        producer: Mutex<rdkafka::producer::FutureProducer>,
    ) -> Arc<Self> {
        Arc::new(State {
            config: config,
            dbconn: dbconn,
            producer: producer,
        })
    }

    pub fn dbconn(&self) -> MutexGuard<postgres::Connection> {
        self.dbconn
            .lock()
            .expect("bug - cannot obtain lock correctly")
    }

    pub fn producer(&self) -> MutexGuard<rdkafka::producer::FutureProducer> {
        self.producer
            .lock()
            .expect("bug - cannot obtain lock correctly")
    }
}
