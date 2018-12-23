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
    print!("{}", repeat(" ").take(indent).collect::<String>());
    match node.data {
        NodeData::Document => println!("#Document"),

        NodeData::Doctype {
            ref name,
            ref public_id,
            ref system_id,
        } => println!("<!DOCTYPE {} \"{}\" \"{}\">", name, public_id, system_id),

        NodeData::Text { ref contents } => {
            println!("#text: {}", escape_default(&contents.borrow()))
        },

        NodeData::Comment { ref contents } => println!("<!-- {} -->", escape_default(contents)),

        NodeData::Element {
            ref name,
            ref attrs,
            ..
        } => {
            assert!(name.ns == ns!(html));
            print!("<{}", name.local);
            for attr in attrs.borrow().iter() {
                assert!(attr.name.ns == ns!());
                print!(" {}=\"{}\"", attr.name.local, attr.value);
            }
            println!(">");
        },

        NodeData::ProcessingInstruction { .. } => unreachable!(),
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
    let body = reqwest::get("https://www.24fa.com/mn59605c49p9.aspx")?.text()?;

    // println!("body = {:?}", body);

    let dom = parse_document(RcDom::default(), Default::default())
        .from_utf8()
        .read_from(body)?;
    walk(0, dom.document);

    if !dom.errors.is_empty() {
        println!("\nParse errors:");
        for err in dom.errors.into_iter() {
            println!("    {}", err);
        }
    }

    Ok(())
}