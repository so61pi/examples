// https://rust-lang.github.io/rust-bindgen/tutorial-4.html

// Include the content of generated bindings into this file.
// To find its path, use
//     find -name bindings.rs
include!(concat!(env!("OUT_DIR"), "/bindings.rs"));

fn main() {
    let name = std::ffi::CString::new("World").unwrap();
    unsafe {
        hello(name.as_ptr());
    }
}
