pub mod implref;
pub mod nonone;
pub mod traitmethodconflict;

pub fn proxy_enum_for_trait(positive: bool) -> impl Iterator<Item = i64> {
    enum ProxyIterator<T, U> {
        Range(T),
        Map(U),
    }

    impl<T: Iterator<Item = i64>, U: Iterator<Item = i64>> Iterator for ProxyIterator<T, U> {
        type Item = i64;

        fn next(&mut self) -> Option<Self::Item> {
            match self {
                ProxyIterator::Range(it) => it.next(),
                ProxyIterator::Map(it) => it.next(),
            }
        }
    }

    let range = 0..3i64;
    if positive {
        ProxyIterator::Range(range)
    } else {
        ProxyIterator::Map(range.map(|e| -e))
    }
}

pub fn return_from_loop() {
    let mut counter = 0;
    let result = loop {
        counter += 1;

        if counter == 10 {
            break counter * 2;
        }
    };
    println!("result is {}", result);

    // We can limit the scope of counter by creating a wrapper block around loop and
    // place counter in it.
    //
    //     let result = {
    //         let mut counter = 0;
    //         loop {
    //             counter += 1;
    //
    //             if counter == 10 {
    //                 break counter * 2;
    //             }
    //         }
    //     };
}

pub fn matching() {
    //
    // match
    //
    {
        {
            // match value
            let number = 13;
            match number {
                // Match a single value.
                1 => println!("One!"),

                // Match several values.
                2 | 3 | 5 | 7 | 11 => println!("This is a prime"),

                // Match an inclusive range.
                //
                // ..= is same as ... (3 dots), but ... is a deprecated syntax so don't use it.
                // For matching, we still only have ..= for inclusive range. The exclusive range
                // syntax (..) issue is at https://github.com/rust-lang/rust/issues/37854
                13..=19 => println!("A teen"),

                // Bind match value to a variable.
                n @ 100..=999 => println!("{} is a big number", n),

                // Matching with guards.
                //
                // Because they are placed below other arms, they are only considered after all
                // the above fail.
                n if n % 2 == 0 => println!("An even number"),
                n if n % 2 != 0 => println!("An odd number"),

                // Handle the rest of cases.
                _ => println!("Impossible"),
            }
        }

        {
            // match tuple
            let pair = (0, -2);
            match pair {
                (0, y) => println!("First is `0` and `y` is `{:?}`", y),
                (x, 0) => println!("`x` is `{:?}` and last is `0`", x),
                _ => println!("It doesn't matter what they are"),
                // `_` means don't bind the value to a variable
            }
            match pair {
                (0, _) => println!("First is `0`"),
                (_, _) => println!("Both are something..."),
                _ => println!("Impossible to get this case"),
            }
        }

        {
            // match enum
            enum Color {
                // These 3 are specified solely by their name.
                Red,
                Blue,
                Green,

                // These likewise tie `u32` tuples to different names: color models.
                RGB(u32, u32, u32),
                HSV(u32, u32, u32),
                HSL(u32, u32, u32),
                CMY(u32, u32, u32),
                CMYK(u32, u32, u32, u32),
            }

            let color = Color::RGB(122, 17, 40);
            match color {
                Color::Red => println!("The color is Red!"),
                Color::Blue => println!("The color is Blue!"),
                Color::Green => println!("The color is Green!"),

                Color::RGB(r, g, b) => println!("Red: {}, green: {}, and blue: {}!", r, g, b),
                Color::HSV(h, s, v) => println!("Hue: {}, saturation: {}, value: {}!", h, s, v),
                Color::HSL(h, s, l) => println!("Hue: {}, saturation: {}, lightness: {}!", h, s, l),
                Color::CMY(c, m, y) => println!("Cyan: {}, magenta: {}, yellow: {}!", c, m, y),
                Color::CMYK(c, m, y, k) => println!(
                    "Cyan: {}, magenta: {}, yellow: {}, key (black): {}!",
                    c, m, y, k
                ),
                // Don't need another arm because all variants have been examined
            }
        }

        {
            // match pointer/ref
            let number = 4;
            let reference = &number;
            match reference {
                // If `reference`s is pattern matched against `&val`, it results
                // in a comparison like:
                // `&i32`
                // `&val`
                // ^ We see that if the matching `&`s are dropped, then the `i32`
                // should be assigned to `val`.
                &val => println!("Got a value via destructuring: {:?}", val),

                // We can also use old ref syntax like `ref val => println!(...)`
                // but it's more verbose and mostly deprecated.
            }

            let mut value = Some(5);
            match &mut value {
                // val is &mut
                Some(val) => *val = 6,

                _ => (),
            }
            println!("New value is {}", value.unwrap());

            struct Foo(i32, i32);
            let mut foo = Foo(7, 8);
            match &mut foo {
                // val0 and val1 are &mut
                Foo(val0, val1) => {
                    *val0 = 9;
                    *val1 = 10;
                }
            }
            println!("New value is {} and {}", foo.0, foo.1);
        }

        {
            // match struct
            struct Foo {
                x: (u32, u32),
                y: u32,
            }

            let foo = Foo { x: (1, 2), y: 3 };

            // Destructure members of the struct.
            let Foo { x: (a, b), y: c } = foo;
            println!("a = {}, b = {}, c = {} ", a, b, c);

            // If the field names are same as ones in the struct, we can bind them directly.
            let Foo { x, y } = foo;
            println!("x = {:?}, y = {} ", x, y);

            // Ignore some fields are possible too.
            let Foo { y, .. } = foo;
            println!("y = {} ", y);

            // We can do the same for match.
            match foo {
                Foo { x, y } => println!("x = {:?}, y = {} ", x, y),
            }
        }
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use std::iter::FromIterator;

    #[test]
    fn enum_iterator_test() {
        assert_eq!(Vec::from_iter(proxy_enum_for_trait(true)), vec![0, 1, 2]);
        assert_eq!(Vec::from_iter(proxy_enum_for_trait(false)), vec![0, -1, -2]);
    }
}
