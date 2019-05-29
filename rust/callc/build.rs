fn main() {
    // https://rust-lang.github.io/rust-bindgen/tutorial-3.html
    // https://doc.rust-lang.org/cargo/reference/build-scripts.html#case-study-building-some-native-code

    // Generate bindings for hello.h
    let bindings = bindgen::Builder::default()
        .header("src/hello.h")
        .generate()
        .expect("Unable to generate bindings");

    // Write the bindings to the $OUT_DIR/bindings.rs file.
    let outpath = std::path::PathBuf::from(std::env::var("OUT_DIR").unwrap());
    bindings
        .write_to_file(outpath.join("bindings.rs"))
        .expect("Couldn't write bindings!");

    // Build hello.c
    cc::Build::new().file("src/hello.c").compile("hello");
}
