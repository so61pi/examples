use actix::AsyncContext;
use actix::StreamHandler;
use futures::sync::oneshot;
use futures::task::Task;
use futures::Future;
use futures::Stream;
use rdkafka::config::ClientConfig;
use rdkafka::consumer::stream_consumer::MessageStream;
use rdkafka::consumer::{CommitMode, Consumer, DefaultConsumerContext, StreamConsumer};
use rdkafka::error::KafkaResult;
use rdkafka::message::Message;
use rdkafka::producer::{DeliveryFuture, FutureProducer, FutureRecord};
use rdkafka::topic_partition_list::TopicPartitionList;
use rdkafka::Offset;
use std::collections::HashMap;
use std::marker::PhantomPinned;
use std::ops::Deref;
use std::pin::Pin;
use std::sync::atomic::AtomicBool;
use std::sync::atomic::AtomicUsize;
use std::sync::atomic::Ordering;
use std::sync::Arc;
use std::sync::Mutex;

pub fn produce(producer: &FutureProducer, topic: &str, key: &str, message: &str) -> DeliveryFuture {
    producer.send(
        FutureRecord::to(topic).payload(message).key(key),
        0, /* do not wait when the queue is full */
    )
    // .then(|v| {
    //     match &v {
    //         Err(e) => log::error!("{}", e),
    //         Ok(Err((e, _message))) => log::error!("{}", e),
    //         Ok(Ok((partition, offset))) => {
    //             log::info!("partition = {}, offset = {}", partition, offset)
    //         }
    //     }
    //     v
    // })
}

pub fn mkproducer(brokers: &str) -> KafkaResult<FutureProducer> {
    let producer: FutureProducer = ClientConfig::new()
        .set("bootstrap.servers", brokers)
        .set("produce.offset.report", "true")
        .set("message.timeout.ms", "5000")
        // .set("debug", "broker,topic,msg,consumer")
        .create()?;
    Ok(producer)
}

pub type DefaultConsumer = StreamConsumer<DefaultConsumerContext>;

pub fn mkconsumer(brokers: &str, group: &str, topics: &[&str]) -> KafkaResult<DefaultConsumer> {
    let consumer: DefaultConsumer = ClientConfig::new()
        .set("bootstrap.servers", brokers)
        .set("group.id", group)
        .set("enable.partition.eof", "false")
        .set("session.timeout.ms", "6000")
        .set("enable.auto.commit", "false")
        .set("queued.min.messages", "20")
        .set("queued.max.messages.kbytes", "64")
        // .set("session.timeout.ms", "900000")
        // .set("request.timeout.ms", "900000")
        // .set("debug", "broker,topic,msg,consumer")
        .create()?;

    consumer.subscribe(topics)?;
    Ok(consumer)
}

#[derive(Debug)]
pub enum HandlerError {
    Redo(failure::Error),
    Stop(failure::Error),
}

impl HandlerError {
    pub fn stop<T>(e: T) -> HandlerError
    where
        T: Into<failure::Error>,
    {
        HandlerError::Stop(e.into())
    }

    pub fn redo<T>(e: T) -> HandlerError
    where
        T: Into<failure::Error>,
    {
        HandlerError::Redo(e.into())
    }
}

impl std::fmt::Display for HandlerError {
    fn fmt(&self, f: &mut std::fmt::Formatter) -> std::fmt::Result {
        match self {
            HandlerError::Redo(e) | HandlerError::Stop(e) => failure::Error::fmt(e, f),
        }
    }
}

pub type HandlerResult<T> = Result<T, HandlerError>;

// The reason we use `Future<Item = (), Error = HandlerError>` instead of
// `Future<Item = (), Error = failure::Error>` is we want to handle the Stop/Redo here
// instead of putting this task on caller side.
pub type Handler = Box<Fn(&str) -> Box<dyn Future<Item = (), Error = HandlerError>>>;
pub type HandlerMap = HashMap<String, Handler>;

fn handle_message(
    consumer: Pin<Arc<PinDefaultConsumer>>,
    handlers: &HandlerMap,
    m: rdkafka::error::KafkaResult<rdkafka::message::OwnedMessage>,
) -> Box<dyn Future<Item = (), Error = ()>> {
    fn handle(
        m: &rdkafka::message::OwnedMessage,
        handlers: &HandlerMap,
    ) -> Box<dyn Future<Item = (), Error = HandlerError>> {
        let payload = match m.payload_view::<str>() {
            None => return Box::new(futures::future::ok(())),
            Some(Err(e)) => {
                log::warn!("error while deserializing message payload: '{:?}'", e);
                return Box::new(futures::future::ok(()));
            }
            Some(Ok(s)) => s,
        };

        match handlers.get(m.topic()) {
            None => {
                let msg = format!("missing handler for topic '{}'", m.topic());
                Box::new(futures::future::err(failure::err_msg(msg)).map_err(HandlerError::stop))
            }
            Some(handler) => handler(payload),
        }
    }

    if let Err(e) = &m {
        log::warn!("kafka error occurred {}", e);
        return Box::new(futures::future::ok(()));
    }
    let m = m.unwrap();

    let fut = handle(&m, handlers).then({
        let mut tp = TopicPartitionList::new();
        tp.add_partition_offset(m.topic(), m.partition(), Offset::Offset(m.offset()));
        let commit = move || {
            if let Err(e) = consumer.commit(&tp, CommitMode::Sync) {
                log::error!("error when committing message '{}'", e);
                Err(())
            } else {
                Ok(())
            }
        };
        move |result| match result {
            Ok(_) => commit(),
            Err(HandlerError::Stop(e)) => {
                log::warn!(
                    "commit message for topic '{}' with error '{}'",
                    m.topic(),
                    e
                );
                commit()
            }
            Err(HandlerError::Redo(e)) => {
                log::warn!("redo message for topic '{}' with error '{}'", m.topic(), e);
                Err(())
            }
        }
    });

    Box::new(fut)
}

struct PinDefaultConsumer {
    consumer: DefaultConsumer,
    _pin: PhantomPinned,
}

impl PinDefaultConsumer {
    fn new(consumer: DefaultConsumer) -> Pin<Arc<PinDefaultConsumer>> {
        Arc::pin(PinDefaultConsumer {
            consumer: consumer,
            _pin: PhantomPinned,
        })
    }
}

impl Deref for PinDefaultConsumer {
    type Target = DefaultConsumer;

    fn deref(&self) -> &Self::Target {
        &self.consumer
    }
}

// TODO: Document why we use 'static, Pin and Arc (but not Rc)...
pub struct OwnedConsumerStream {
    _consumer: Pin<Arc<PinDefaultConsumer>>,
    stream: MessageStream<'static, DefaultConsumerContext>,
    pause: Arc<AtomicBool>,
    task: Arc<Mutex<Option<Task>>>,
}

impl OwnedConsumerStream {
    fn new(
        consumer: Pin<Arc<PinDefaultConsumer>>,
        pause: Arc<AtomicBool>,
        task: Arc<Mutex<Option<Task>>>,
    ) -> OwnedConsumerStream {
        let pc: *const DefaultConsumer = &consumer as &DefaultConsumer;
        let s = unsafe { &*pc }; // erase lifetime (as pointers have no associated lifetime)
        let stream = s.start();
        OwnedConsumerStream {
            _consumer: consumer,
            stream: stream,
            pause: pause,
            task: task,
        }
    }
}

impl Stream for OwnedConsumerStream {
    type Item = KafkaResult<rdkafka::message::OwnedMessage>;
    type Error = ();

    fn poll(&mut self) -> futures::Poll<Option<Self::Item>, Self::Error> {
        let mut task = self.task.lock().unwrap();
        if self.pause.load(Ordering::SeqCst) {
            task.get_or_insert_with(futures::task::current);
            Ok(futures::Async::NotReady)
        } else {
            task.take();
            self.stream.poll().map(|ready| {
                ready.map(|option| option.map(|result| result.map(|borrowed| borrowed.detach())))
            })
        }
    }
}

pub struct ConsumerActor {
    consumer: Pin<Arc<PinDefaultConsumer>>,
    handlers: HandlerMap,
    receiver: oneshot::Receiver<()>,
    pause: Arc<AtomicBool>,
    queued: Arc<AtomicUsize>,
    task: Arc<Mutex<Option<Task>>>,
}

impl ConsumerActor {
    pub fn new(consumer: DefaultConsumer, handlers: HandlerMap) -> ConsumerActor {
        let (sender, receiver) = oneshot::channel::<()>();
        sender
            .send(())
            .expect("sender shouldn't fail here as receiver is still alive");
        ConsumerActor {
            consumer: PinDefaultConsumer::new(consumer),
            handlers: handlers,
            receiver: receiver,
            pause: Arc::new(AtomicBool::new(false)),
            queued: Arc::new(AtomicUsize::new(0)),
            task: Arc::new(Mutex::new(None)),
        }
    }
}

impl actix::StreamHandler<KafkaResult<rdkafka::message::OwnedMessage>, ()> for ConsumerActor {
    fn handle(
        &mut self,
        item: KafkaResult<rdkafka::message::OwnedMessage>,
        ctx: &mut actix::Context<ConsumerActor>,
    ) {
        let fut = handle_message(self.consumer.clone(), &self.handlers, item);

        let (sender2, receiver2) = oneshot::channel::<()>();
        let receiver = std::mem::replace(&mut self.receiver, receiver2);
        let fut2 = receiver
            .map_err(|_| ())
            .and_then(|_| {
                fut.and_then(move |_| {
                    sender2
                        .send(())
                        .expect("sender shouldn't fail here as receiver is still alive");
                    futures::future::ok(())
                })
            })
            .then({
                let queued = self.queued.clone();
                let pause = self.pause.clone();
                let task = self.task.clone();
                move |v| {
                    if queued.fetch_sub(1, Ordering::SeqCst) < 5 {
                        pause.store(false, Ordering::SeqCst);
                        let mut task = task.lock().unwrap();
                        if let Some(t) = task.take() {
                            t.notify();
                        }
                    }
                    v
                }
            });

        if self.queued.fetch_add(1, Ordering::SeqCst) > 20 {
            self.pause.store(true, Ordering::SeqCst);
        }
        ctx.spawn(actix::fut::wrap_future(fut2));
    }
}

impl actix::Actor for ConsumerActor {
    type Context = actix::Context<Self>;

    fn started(&mut self, ctx: &mut actix::Context<Self>) {
        Self::add_stream(
            OwnedConsumerStream::new(self.consumer.clone(), self.pause.clone(), self.task.clone()),
            ctx,
        );
    }
}
