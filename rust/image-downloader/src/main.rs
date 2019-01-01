use std::default::Default;
use std::io;
use std::iter::repeat;
use std::string::String;

use html5ever::parse_document;
use html5ever::rcdom::{Handle, NodeData, RcDom};
use html5ever::tendril::TendrilSink;

// This is not proper HTML serialization, of course.

fn walk(indent: usize, handle: Handle) {
    let node = handle;
    // FIXME: don't allocate
    // print!("{}", repeat(" ").take(indent).collect::<String>());
    match node.data {
        // NodeData::Document => println!("#Document"),

        // NodeData::Doctype {
        //     ref name,
        //     ref public_id,
        //     ref system_id,
        // } => println!("<!DOCTYPE {} \"{}\" \"{}\">", name, public_id, system_id),

        // NodeData::Text { ref contents } => {
        //     println!("#text: {}", escape_default(&contents.borrow()))
        // },

        // NodeData::Comment { ref contents } => println!("<!-- {} -->", escape_default(contents)),

        NodeData::Element {
            ref name,
            ref attrs,
            ..
        } => {
            // TODO: Converting below if statement to a match.
            if name.local.eq_str_ignore_ascii_case("a") || name.local.eq_str_ignore_ascii_case("img") {
                print!("<{}", name.local);
                for attr in attrs.borrow().iter() {
                    print!(" {}=\"{}\"", attr.name.local, attr.value);
                }
                println!(">");
            }
        },

        // NodeData::ProcessingInstruction { .. } => unreachable!(),

        _ => {}
    }

    for child in node.children.borrow().iter() {
        walk(indent + 4, child.clone());
    }
}

// FIXME: Copy of str::escape_default from std, which is currently unstable
pub fn escape_default(s: &str) -> String {
    s.chars().flat_map(|c| c.escape_default()).collect()
}

fn main() -> Result<(), Box<dyn std::error::Error>> {
    let body = reqwest::get("https://www.pexels.com/search/HD%20wallpaper/")?.text()?;

    println!("body = {:?}", body);

    let dom = parse_document(RcDom::default(), Default::default())
        .from_utf8()
        .read_from(&mut body.as_bytes())?;
    walk(0, dom.document);

    if !dom.errors.is_empty() {
        println!("\nParse errors:");
        for err in dom.errors.into_iter() {
            println!("    {}", err);
        }
    }

    Ok(())
}
