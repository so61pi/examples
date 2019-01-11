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

/*
URLROOT
| ID | URL | PERIOD |

URL
| ID | URL | ENCOUNTER(U64) | DOWNLOADCOUNT(U64) | LASTDOWNLOADTIME | COMPLETED | TYPE |

IMGURL
| ID | FILENAME | FILESIZE | FILEHASH | IMGTYPE | IMGWIDTH | IMGHEIGHT |
*/


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
    debug!("fetching html {}!", url);

    let body = client.get(url).send()?.text()?;

    // println!("body = {:?}", body);

    let dom = html5ever::parse_document(html5ever::rcdom::RcDom::default(), Default::default())
        .from_utf8()
        .read_from(&mut body.as_bytes())?;

    let mut htmlurls = Vec::new();
    let mut imgurls = Vec::new();
    walkhtml(0, dom.document, htmlurls, imgurls);

    debug!("htmlurls {}", htmlurls);
    debug!("imgurls {}", imgurls);

    if !dom.errors.is_empty() {
        println!("\nParse errors:");
        for err in dom.errors.into_iter() {
            println!("    {}", err);
        }
    }

    Ok(htmlurls, imgurls)
}

fn fetchhtmlx(client : &reqwest::Client, mutex : &std::sync::Mutex, url : Url) -> Result<(Vec<String>, Vec<String>)> {
    queueurllock(mutex, url)?;
    let (htmlurls, imgurls) = fetchhtml(client, url);

    fn save(urls : &Vec<String>) -> Result<Vec<String>> {
        let uniques = Vec::new();
        for url in &urls {
            let existed = queueurllock(mutex, url)?;
            if !existed {
                uniques.push(url);
            }
        }
        Ok(uniques)
    }

    Ok((save(htmlurls)?, save(imgurls)?))
}

fn fetchimg(client : &reqwest::Client, url : String) -> Result<Vec<u8>> {
    debug!("fetching image {}!", url);

    let mut resp = client.get(url).send()?;
    let mut result: Vec<u8> = vec![];
    resp.copy_to(&mut result);
    Ok(result)
}


fn fetchimgx(client : &reqwest::Client, mutex : &std::sync::Mutex, url: &String) -> Result<Vec<u8>> {
    debug!("fetching image {}!", url);

    queueurllock(mutex, url)?;
    Ok(result)
}


fn queueurl(url : Url) -> Result<bool> {
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

fn queueurllock(mutex: &std::sync::Mutex, storage: &Storage, url: &String, type: UrlType) -> Result<Option<u64>> {
    let lock = mutex.lock()?;

    if let Some(time) = storage.lastdownloadtime(url)? {
        if SystemTime::now() > (time + 10h) {
            storage.queue(url, type);
        }
    } else {
        storage.queue(url, type);
    }

}


fn abc(client: &reqwest::Client, mutex : &std::sync::Mutex, htmlurls : &Vec<String>) {
    fn xxx(mutex : &std::sync::Mutex, htmlurl : String) {
        let (htmlurls, imgurls) = fetchhtmlx(mutex, client, htmlurl);

        let mutex1 = mutex.clone();
        rayon::spawn(move || abc(mutex1, htmlurls));

        let mutex2 = mutex.clone();
        rayon::spawn(move || def(mutex2, imgurls));
    }

    for htmlurl in &htmlurls {
        let mutex = mutex.clone();
        rayon::spawn(move || xxx(mutex, htmlurl));
    }
}

fn def(client: reqwest::Client, mutex: &std::sync::Mutex, imgurls : &Vec<String>) {
    for imgurl in &imgurls {
        let mutex = mutex.clone();
        rayon::spawn(move || fetchimgx(mutex, client, imgurl));
    }
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

    let mutex = Arc::new(Mutex::new(0));
    loop {
        let rooturls = getroots();
        abc(mutex, rooturls);
        thread::sleep(1s);
    }

    Ok(())
}
