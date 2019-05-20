//! Example document
//!
//! Document for crate starts with `//!`.
//!
//! ```should_panic
//! // Panic ahead.
//! docs::div(10, 0);
//! ```

pub mod test {
    //! Document for module starts with `//!`.
    //!
    //! Similar to crate-wise document.

    /// Document for function starts with `///`.
    pub fn print(s: &str) {
        print!("{}", s);
    }

    /// Similar to function.
    pub struct Test {}

    /// impl block can have document and doctest too.
    ///
    /// ```
    /// let result = docs::test::Test::add(1, 2);
    /// assert_eq!(3, result);
    /// ```
    impl Test {
        /// Create new object, obviously.
        pub fn new() -> Test {
            Test{}
        }

        /// Add two numbers.
        pub fn add(a: i32, b: i32) -> i32 {
            a + b
        }
    }
}

/// This function divides two numbers.
///
/// # Examples
///
/// ```
/// // The line `# use docs::div;` will be omitted during document generating.
/// # use docs::div;
/// let result = div(10, 2);
/// assert_eq!(result, 5);
/// ```
///
/// # Panics
///
/// The function panics if the second argument is zero.
///
/// ```rust,should_panic
/// docs::div(10, 0);
/// ```
///
/// # Compilation fails
///
/// ```compile_fail
/// docs::div(10, 0, 0);
/// ```
///
/// # Test for compilable code but will not run
///
/// ```no_run
/// loop {
///     println!("Hello, world");
/// }
/// ```
///
/// Read more at https://doc.rust-lang.org/rustdoc/index.html
pub fn div(a: i32, b: i32) -> i32 {
    if b == 0 {
        panic!("Divide-by-zero error");
    }

    a / b
}
