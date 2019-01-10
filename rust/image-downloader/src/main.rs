use std::default::Default;
use std::io;
use std::iter::repeat;
use std::string::String;
use std::sync::mpsc::{Sender, Receiver};
use std::sync::mpsc;
use std::thread;

// use html5ever::parse_document;
// use html5ever::rcdom::{Handle, NodeData, RcDom};
use html5ever::tendril::TendrilSink;

// This is not proper HTML serialization, of course.

fn walkhtml(handle: html5ever::rcdom::Handle, htmlurls: &mut Vec<String>, imgurls: &mut Vec<String>) {
    let node = handle;
    match node.data {
        // html5ever::rcdom::NodeData::Document => println!("#Document"),

        // html5ever::rcdom::NodeData::Doctype {
        //     ref name,
        //     ref public_id,
        //     ref system_id,
        // } => println!("<!DOCTYPE {} \"{}\" \"{}\">", name, public_id, system_id),

        // html5ever::rcdom::NodeData::Text { ref contents } => {
        //     println!("#text: {}", escape_default(&contents.borrow()))
        // },

        // html5ever::rcdom::NodeData::Comment { ref contents } => println!("<!-- {} -->", escape_default(contents)),

        html5ever::rcdom::NodeData::Element {
            ref name,
            ref attrs,
            ..
        } => {
            // TODO: Converting below if statement to a match.
            match name.local {
                html5ever::local_name!("a") => {
                    htmlurls.push("TODO: Extract URL");
                },
                html5ever::local_name!("img") => {
                    for attr in attrs.borrow().iter() {
                        imgurls.push("TODO: Extract URL");
                    }
                },
                _ => {}
            };
        },

        // html5ever::rcdom::NodeData::ProcessingInstruction { .. } => unreachable!(),

        _ => {}
    }

    for child in node.children.borrow().iter() {
        walkhtml(child.clone(), htmlurls, imgurls);
    }
}

fn fetchhtml(client : &reqwest::Client, url : String) -> Result<(Vec<String>, Vec<String>)> {
    let body = client.get(url)
        .send()?.text()?;

    // println!("body = {:?}", body);

    let dom = html5ever::parse_document(html5ever::rcdom::RcDom::default(), Default::default())
        .from_utf8()
        .read_from(&mut body.as_bytes())?;

    let mut htmlurls = Vec::new();
    let mut imgurls = Vec::new();
    walkhtml(0, dom.document, htmlurls, imgurls);

    if !dom.errors.is_empty() {
        println!("\nParse errors:");
        for err in dom.errors.into_iter() {
            println!("    {}", err);
        }
    }

    Ok(htmlurls, imgurls)
}

fn fetchimgx(client : &reqwest::Client, url : String) -> Result<Vec<u8>> {
    Err("asd")
}

// FIXME: Copy of str::escape_default from std, which is currently unstable
// pub fn escape_default(s: &str) -> String {
//     s.chars().flat_map(|c| c.escape_default()).collect()
// }


static NTHREADS: i32 = 4;

fn dbthread(a : Receiver<i32>) {
    // Channels have two endpoints: the `Sender<T>` and the `Receiver<T>`,
    // where `T` is the type of the message to be transferred
    // (type annotation is superfluous)
    let mut children = Vec::new();

    for id in 0..NTHREADS {
        // The sender endpoint can be copied
        let thread_tx = tx.clone();

        // Each thread will send its id via the channel
        let child = thread::spawn(move || {
            // The thread takes ownership over `thread_tx`
            // Each thread queues a message in the channel
            thread_tx.send(id).unwrap();

            // Sending is a non-blocking operation, the thread will continue
            // immediately after sending its message
            println!("thread {} finished", id);
        });

        children.push(child);
    }

    // Here, all the messages are collected
    let mut ids = Vec::with_capacity(NTHREADS as usize);
    for _ in 0..NTHREADS {
        // The `recv` method picks a message from the channel
        // `recv` will block the current thread if there are no messages available
        ids.push(rx.recv());
    }

    // Wait for the threads to complete any remaining work
    for child in children {
        child.join().expect("oops! the child thread panicked");
    }

    // Show the order in which the messages were sent
    println!("{:?}", ids);
}


fn saveurl(url : Url) -> Result<bool> {
    /*
    - Find URL in DB
        - Found
            - Save URL to DB (url, count++, accesstime=now)
            - return true
        - NotFound
            - Save URL to DB (url, completed=false, count=1, starttime=now, completetime=0, accesstime=now, type)
            - return false
    */
}

fn fetchhtmlx(client, url : Url) -> Result<(Vec<String>, Vec<String>)> {
    /*
    - saveurl(url);
    - let (htmlurls, imgurls) = fetchhtml(client, url)
    - Save <a> to DB
        Found:
            - Save URL to DB (url, count++, accesstime=now)
            - Return
        NotFound:
            - Save URL to DB (url, completed=false, count=0, starttime=now, completetime=0, accesstime=now)
    - Save <img> to DB
        Found:
            - Save URL to DB (url, count++, accesstime=now)
            - Return
        NotFound:
            - Save URL to DB (url, completed=false, count=0, starttime=now, completetime=0, accesstime=now,
                              filename="/dev/null", filesize=U64(0), filehash="none:",
                              imgtype="none", imgwidth=0, imgheight=0)
    */
    saveurl(url);
    let (htmlurls, imgurls) = fetchhtml(client, url);

    fn save(urls : &Vec<String>) -> Result<Vec<String>> {
        let uniques = Vec::new();
        for url in &urls {
            let existed = saveurl(htmlurl)?;
            if !existed {
                uniques.push(url);
            }
        }
        Ok(uniques)
    }

    Ok((save(htmlurls)?, save(imgurls)?))
}

// TODO: Clone client or change to tx, rx with a fetching thread
fn abc(htmlurls : &Vec<String>) {
    fn xxx(htmlurl : String) {
        let (htmlurls, imgurls) = fetchhtmlx(client, htmlurl);
        rayon::spawn(move || abc(htmlurls));
        rayon::spawn(move || def(imgurls));
    }

    for htmlurl in &htmlurls {
        rayon::spawn(move || xxx(htmlurl));
    }
}

fn def(imgurls : &Vec<String>) {
    for imgurl in &imgurls {
        rayon::spawn(move || fetchimgx(client, imgurl));
    }
}

fn addroot(url : String, period /* seconds */ : u32) {
    // TODO
}


fn main() -> Result<(), Box<dyn std::error::Error>> {
    // let body = reqwest::get("https://www.pexels.com/search/HD%20wallpaper/")?.text()?;

    fn mkclient() -> reqwest::Result<reqwest::Client> {
        match std::env::var("http_proxy") {
            Ok(val) => {
                println!("proxy = {}", val);
                reqwest::Client::builder().proxy(reqwest::Proxy::http(&val)?).build()
            },
            Err(_) => {
                reqwest::Client::builder().build()
            }
        }
    }
    let client = mkclient()?;

    loop {
        let rooturls = getroots();
        abc(rooturls);
        thread::sleep(1s);
    }

    Ok(())
}
