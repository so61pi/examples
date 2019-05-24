use html5ever::tendril::TendrilSink;

pub struct LevelUrl {
    pub level: u32,
    pub url: url::Url,
}

impl LevelUrl {
    fn new(level: u32, url: url::Url) -> Self {
        LevelUrl {
            level: level,
            url: url,
        }
    }
}

pub struct CollectedUrls {
    pub media: Vec<LevelUrl>,
    pub html: Vec<LevelUrl>,
    pub discarded: Vec<String>,
}

impl CollectedUrls {
    fn new() -> Self {
        CollectedUrls {
            media: vec![],
            html: vec![],
            discarded: vec![],
        }
    }
}

pub fn geturls(client: &reqwest::Client, url: &url::Url) -> failure::Fallible<CollectedUrls> {
    let body = client.get(url.as_str()).send()?.text()?;

    let dom = html5ever::parse_document(html5ever::rcdom::RcDom::default(), Default::default())
        .from_utf8()
        .read_from(&mut body.as_bytes())?;

    let mut urls = CollectedUrls::new();
    walkhtml(0, url, dom.document, &mut urls);

    fn unique(vec: &mut Vec<LevelUrl>) {
        vec.sort_unstable_by(|a, b| a.url.cmp(&b.url));
        vec.dedup_by(|a, b| a.url == b.url);
        vec.sort_unstable_by(|a, b| b.level.cmp(&a.level));
    }

    unique(&mut urls.html);
    unique(&mut urls.media);
    urls.discarded.sort_unstable();
    urls.discarded.dedup();

    // if !dom.errors.is_empty() {
    //     log::info!("html parse errors");
    //     for err in dom.errors.iter() {
    //         log::info!("- {}", err);
    //     }
    // }

    Ok(urls)
}

fn walkhtml(
    level: u32,
    base: &url::Url,
    handle: html5ever::rcdom::Handle,
    urls: &mut CollectedUrls,
) {
    let node = handle;
    if let html5ever::rcdom::NodeData::Element {
        name,
        attrs,
        ..
    } = &node.data {
        fn getattr(attr: &html5ever::interface::Attribute, name: &str) -> Option<String> {
            // The type of `local` is `Atom<LocalNameStaticSet>` which only implements
            // `PartialEq<str>` but not `PartialEq<&str>`. Therefore, instead of writing
            // `attr.name.local == name` (which uses `PartialEq<&str>`), we have to use
            // `*name` to get the type `str`.
            //
            // Alternatively, we can also write the below comparison as `&attr.name.local == name`.
            // Note that even though we are comparing references, this is different from comparing
            // C/C++ pointers and more like C++ references. That means rust will correctly use
            // `PartialEq<str>`.
            if attr.name.local == *name {
                Some(attr.value.to_string())
            } else {
                None
            }
        }

        // prevent URIs like src="data:image/svg" or href="javascript:void(0)"
        fn validate(u: &url::Url) -> bool {
            !(u.cannot_be_a_base()
                || u.host() == None
                || !(u.scheme() == "http" || u.scheme() == "https"))
        }

        fn tourl(base: &url::Url, s: &str) -> Option<url::Url> {
            url::Url::parse(s)
                .or_else(|e| {
                    if e == url::ParseError::RelativeUrlWithoutBase {
                        base.join(s)
                    } else {
                        Err(e)
                    }
                })
                .or_else(Err)
                .ok()
                .filter(validate)
        }

        match name.local {
            html5ever::local_name!("a") => {
                attrs
                    .borrow()
                    .iter()
                    .filter_map(|attr| getattr(attr, "href"))
                    .map(|url| tourl(base, &url).ok_or(url))
                    .for_each(|result| match result {
                        Ok(v) => urls.html.push(LevelUrl::new(level, v)),
                        Err(v) => urls.discarded.push(v),
                    });
            }
            html5ever::local_name!("img") | html5ever::local_name!("video") => {
                // TODO: srcset
                attrs
                    .borrow()
                    .iter()
                    .filter_map(|attr| getattr(attr, "src"))
                    .map(|url| tourl(base, &url).ok_or(url))
                    .for_each(|result| match result {
                        Ok(v) => urls.media.push(LevelUrl::new(level, v)),
                        Err(v) => urls.discarded.push(v),
                    });
            }
            _ => {}
        };
    }

    for child in node.children.borrow().iter() {
        walkhtml(level + 1, base, child.clone(), urls);
    }
}
