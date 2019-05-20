.. contents:: Table of Contents

Generic
=======

Generic Type/Lifetime Declaration
---------------------------------

Functions
~~~~~~~~~

.. code-block:: rust

    //      declaration
    //        ~~~~~~~
    fn example<'a, T>(data: &'a [T]) {}

Structs
~~~~~~~

.. code-block:: rust

    //          declaration
    //            ~~~~~~~
    struct Example<'a, T> {
        data: &'a T,
    }

Enums
~~~~~

.. code-block:: rust

    //        declaration
    //          ~~~~~~~
    enum Example<'a, T> {
        A(&'a T),
        B(T),
    }

Methods
~~~~~~~

.. code-block:: rust

    //  declaration
    //  ~~~~~~~
    impl<'a, T> Example<'a, T> {
        fn new(dt: &T) -> Example<T> {
            Example{
                data: dt,
            }
        }
    }

Note that the above snippet can be rewitten using **lifetime elision** and **anonymous lifetime** like below:

.. code-block:: rust

    //  (1)         (2)
    impl<T> Example<'_, T> {
        //                       (3)
        fn new(dt: &T) -> Example<T> {
            Example{
                data: dt,
            }
        }
    }

- At (1) and (3), lifetime can be omitted completely as per **lifetime elision** rules.

  See https://doc.rust-lang.org/edition-guide/rust-2018/ownership-and-lifetimes/lifetime-elision-in-impl.html

- At (2), however, we must at least put an **anonymous lifetime** there as Rust-2018 requires that.

  See https://doc.rust-lang.org/edition-guide/rust-2018/ownership-and-lifetimes/the-anonymous-lifetime.html#more-details

See more at `Lifetime Elision`_.

Traits
~~~~~~

.. code-block:: rust

    //         declaration
    //           ~~~~~~~
    trait Example<'a, T> {}

Bounds
------

Generic items can use other traits as bounds. We can put traits where the generic types are declared, or using where clause.

.. code-block:: rust

    fn example<T: std::fmt::Display>(t: T) {}
    fn example<T>(t: T) where T: std::fmt::Display {}

    struct Example<T: std::fmt::Display>(T);
    struct Example<T>(T) where T: std::fmt::Display;

    struct Example<T: std::fmt::Display> { data: T }
    struct Example<T> where T: std::fmt::Display { data: T }

    impl<T: std::fmt::Display> Example<T> {}
    impl<T> Example<T> where T: std::fmt::Display {}

    trait Circle : Shape {}
    trait Circle where Self : Shape {}

    trait A { type B: Copy; }
    trait A where Self::B: Copy { type B; }

Default Generic Type Parameters
-------------------------------

.. code-block:: rust

    trait Add<RHS=Self> {
        type Output;

        fn add(self, rhs: RHS) -> Self::Output;
    }

    impl Add for Point {
        type Output = Point;

        fn add(self, other: Point) -> Point {
            Point {
                x: self.x + other.x,
                y: self.y + other.y,
            }
        }
    }

    struct Millimeters(u32);
    struct Meters(u32);

    impl Add<Meters> for Millimeters {
        type Output = Millimeters;

        fn add(self, other: Meters) -> Millimeters {
            Millimeters(self.0 + (other.0 * 1000))
        }
    }

References
----------

- https://doc.rust-lang.org/rust-by-example/generics.html
- https://doc.rust-lang.org/1.30.0/book/second-edition/ch10-01-syntax.html
- https://doc.rust-lang.org/reference/items/functions.html
- https://doc.rust-lang.org/reference/items/traits.html
- https://doc.rust-lang.org/reference/items/structs.html
- https://doc.rust-lang.org/reference/items/implementations.html
- https://doc.rust-lang.org/reference/items/generics.html

Lifetime
========

Notes
-----

- Lifetimes are only relevant to references because we can have more than one references pointing to a same variable.

- Pointers in rust don't possess lifetime information.

- Types don't have lifetime because they are meta data, they don't come and go like data. However, they can have lifetime relationships.

  For example, with this struct

  .. code-block:: rust

      pub struct MyStruct<'a> {
          pub value: &'a str,
      }

  we can claim that the lifetime of any instance of ``MyStruct`` must be shorter than or equal to the lifetime of the string ``value`` points to.

- Unbounded lifetime is lifetime that has no relationships with any other lifetimes.

  .. code-block:: rust

    pub struct InvalidStruct<'a> {
        pub value: i32,
    }

  ``InvalidStruct`` must not outlive nothing, so it has unlimited lifetime (more than ``'static`` semantically). ``'a`` is called unbounded lifetime, and rust forbiddens unbounded lifetime on struct.

- Bounded lifetime, on the other hand, has relationships. In other words, it has boundary.

  For example, a reference cannot live longer than the value it points to. Or, ``'static`` lifetime cannot exceed the process' duration.

Lifetime Elision
----------------

Lifetimes on function parameters are called **input lifetime**, and lifetimes on return values are called **output lifetime**.

#. each elided input lifetime that is a reference gets its own lifetime parameter
#. if there is exactly one input lifetime parameter, that lifetime is assigned to all output lifetime parameters
#. if there are multiple input lifetime parameters, but one of them is ``&self`` or ``&mut self`` because this is a method, the lifetime of self is assigned to all output lifetime parameters

====================================  ==================================================  =====
Code (elided form)                    Code (expanded form)                                Rules
====================================  ==================================================  =====
``fn foo(x: &i32)``                   ``fn foo<'a>(x: &'a i32)``                          1
``fn foo(x: &i32, y: &i32)``          ``fn foo<'a, 'b>(x: &'a i32, y: &'b i32)``          1
``fn foo(x: &i32) -> &i32``           ``fn foo<'a>(x: &'a i32) -> &'a i32``               1 + 2
``fn foo(x: &str, y: &str) -> &str``  ``fn foo<'a, 'b>(x: &'a str, y: &'b str) -> &str``  1
``fn foo(&self, x: &i32) -> &str``    ``fn foo<'a, b>(&'a self, x: &'b i32) -> &'a str``  1 + 3
====================================  ==================================================  =====

Same rules apply for ``impl`` headers. In this case, input lifetimes refer to the lifetimes appears in the type receiving the ``impl``.
And the output lifetimes refer to the lifetimes of trait. The following table shows some examples.

======================================================  ==============  ===============
Code                                                    Input lifetime  Output lifetime
======================================================  ==============  ===============
``impl<'a> Foo<'a>``                                    ``'a``
``impl<'a, 'b, 'c> SomeTrait<'b, 'c> for Foo<'a, 'c>``  ``'a`` ``'c``   ``'b`` ``'c``
======================================================  ==============  ===============

The RFC for this is https://github.com/rust-lang/rfcs/blob/master/text/0141-lifetime-elision.md. Some rules in this RFC are not implemented deliberately, see https://github.com/rust-lang/rust/issues/15872#issuecomment-413080939.

Lifetime Subtyping
------------------

Lifetime subtyping specifies that one lifetime parameter lives at least as long as another one. In the angle brackets where we declare lifetime parameters, we can declare a lifetime ``'a`` as usual and declare a lifetime ``'b`` that lives at least as long as ``'a`` by declaring ``'b`` using the syntax ``'b: 'a``.

.. code-block:: rust

    struct Parser<'c, 's: 'c> {
        context: &'c Context<'s>,
    }

Lifetime Bounds on References to Generic Types
----------------------------------------------

``struct Ref<'a, T>(&'a T);``

Because ``T`` can be any type, ``T`` could be a reference or a type that holds one or more references, each of which could have their own lifetimes. Rust can’t be sure ``T`` will live as long as ``'a``.

``struct Ref<'a, T: 'a>(&'a T);``

This code now compiles because the ``T: 'a`` syntax specifies that ``T`` can be any type, but if it contains any references, the references must live at least as long as ``'a``.

Functions That Mess With Lifetime
---------------------------------

- ``std::mem::transmute`` reinterprets the bits of a value of one type as another type so is can be utilized to convert

  - Value of one type to value of another type (``u32`` to ``f32``, ``*u32`` to ``*f32``).
  - Reference of one lifetime to reference of another lifetime (``'a`` to and from ``'static``).

  Note that in case of converting reference/pointer, source and destination must have the same immutability. Otherwise it will result in undefined behavior.

References
----------

- https://doc.rust-lang.org/book/ch10-03-lifetime-syntax.html
- https://doc.rust-lang.org/book/ch19-02-advanced-lifetimes.html
- https://doc.rust-lang.org/std/mem/fn.transmute.html

Trait
=====

Notes
-----

- Traits are for shared behaviors.

- Traits can have default implementations.

- Traits can be used as parameters.

  .. code-block:: rust

    pub fn foo(item: impl Display) {}

    pub fn foo<T: Display>(item: T) {}

    pub fn foo(item: impl Display + Clone) {}

    pub fn foo<T: Display + Clone>(item: T) {}

    pub fn foo<T>(item: T)
        where T: Display + Clone,
    {}

  We can also use ``&impl Display`` or ``&dyn Display`` but there are some differences between them

  - ``&impl Display`` expects a concrete object from the calling side, while ``&dyn Display`` doesn't.

  - Currently, ``&impl`` usage in function parameters supports multiple traits (e.g. ``&impl Display + Clone``), but ``&dyn`` doesn't.

- Traits can be used as return type.

  .. code-block:: rust

    pub fn foo() -> impl Display {}

  This is not for dynamic dispatching, it's just about hiding private types. Also, all return statements must have the same types.

- Conditionally implement methods for a type.

  .. code-block:: rust

    use std::fmt::Display;

    struct Pair<T> {
        x: T,
        y: T,
    }

    impl<T> Pair<T> {
        fn new(x: T, y: T) -> Self {
            Self {
                x,
                y,
            }
        }
    }

    impl<T: Display + PartialOrd> Pair<T> {
        fn cmp_display(&self) {
            if self.x >= self.y {
                println!("The largest member is x = {}", self.x);
            } else {
                println!("The largest member is y = {}", self.y);
            }
        }
    }

- Implement a trait for types that satisfies some traits.

  .. code-block:: rust

    impl<T: Display + Clone> ToString for T {
        // --snip--
    }

  This is blanket implementation.

Associated Types
----------------

Associated types connect a type placeholder with a trait such that the trait method definitions can use these placeholder types in their signatures.

.. code-block:: rust

  pub trait Iterator {
      type Item;

      fn next(&mut self) -> Option<Self::Item>;
  }

When you define ``trait Iterator``, you only have one trait. But with ``trait Iterator<T>``, you have multiple traits (e.g. ``trait Iterator<String>``, ``trait Iterator<i32>``...) and users are free to have different behaviors depending on the provided type (like in the table below).

Use associated types by default to keep things simple and in check.

+---------------+-------------------------------------------------+---------------------------------------------+
|               | Associated Types                                | Type Parameters                             |
+===============+=================================================+=============================================+
| Definition    | .. code-block:: rust                            | .. code-block:: rust                        |
|               |                                                 |                                             |
|               |   trait Iterator {                              |   trait Iterator<T> {                       |
|               |     type Item;                                  |     fn next(&mut self) -> Option<T>;        |
|               |                                                 |   }                                         |
|               |     fn next(&mut self) -> Option<Self::Item>;   |                                             |
|               |   }                                             |                                             |
|               |                                                 |                                             |
+---------------+-------------------------------------------------+---------------------------------------------+
| Impl          | .. code-block:: rust                            | .. code-block:: rust                        |
|               |                                                 |                                             |
|               |   impl Iterator for Counter {                   |   impl<T> Iterator<T> for Counter {         |
|               |     type Item = u32;                            |     fn next(&mut self) -> Option<T> {       |
|               |                                                 |       None                                  |
|               |     fn next(&mut self) -> Option<Self::Item> {  |     }                                       |
|               |        None                                     |   }                                         |
|               |     }                                           |                                             |
|               |   }                                             +---------------------------------------------+
|               |                                                 | .. code-block:: rust                        |
|               |                                                 |                                             |
|               |                                                 |   impl Iterator<String> for Counter {       |
|               |                                                 |     fn next(&mut self) -> Option<String> {  |
|               |                                                 |       panic!("String")                      |
|               |                                                 |     }                                       |
|               |                                                 |   }                                         |
|               |                                                 |                                             |
|               |                                                 |   impl Iterator<i32> for Counter {          |
|               |                                                 |     fn next(&mut self) -> Option<i32> {     |
|               |                                                 |       None                                  |
|               |                                                 |     }                                       |
|               |                                                 |   }                                         |
|               |                                                 |                                             |
+---------------+-------------------------------------------------+---------------------------------------------+

References
----------

- https://doc.rust-lang.org/book/ch10-02-traits.html
- https://doc.rust-lang.org/book/ch19-02-advanced-lifetimes.html

Subtyping and Variance
======================

Notes
-----

- if ``'big: 'small`` ("big contains small" or "big outlives small"), then ``'big`` is a subtype of ``'small``.
- ``'static`` is a subtype of every lifetime because by definition it outlives everything

Variance
--------

Variance is a property that generic types have with respect to their arguments. A generic type's variance in a parameter is how the subtyping of the parameter affects the subtyping of the type.

- ``F<T>`` is **covariant** over ``T`` if ``T`` being a subtype of ``U`` implies that ``F<T>`` is a subtype of ``F<U>`` (subtyping "passes through")
- ``F<T>`` is **contravariant** over ``T`` if ``T`` being a subtype of ``U`` implies that ``F<U>`` is a subtype of ``F<T>``
- ``F<T>`` is **invariant** over ``T`` otherwise (no subtyping relation can be derived)

+---------------------------------+--------------------+-------------------+
| Type                            | Variance in ``'a`` | Variance in ``T`` |
+=================================+====================+===================+
| ``&'a T``                       | **covariant**      | **covariant**     |
+---------------------------------+--------------------+-------------------+
| ``&'a mut T``                   | **covariant**      | invariant         |
+---------------------------------+--------------------+-------------------+
| ``*const T``                    |                    | **covariant**     |
+---------------------------------+--------------------+-------------------+
| ``*mut T``                      |                    | invariant         |
+---------------------------------+--------------------+-------------------+
+---------------------------------+--------------------+-------------------+
| ``fn() -> T``                   |                    | **covariant**     |
+---------------------------------+--------------------+-------------------+
| ``fn(T) -> ()``                 |                    | contravariant     |
+---------------------------------+--------------------+-------------------+
+---------------------------------+--------------------+-------------------+
| ``[T]`` and ``[T; n]``          |                    | **covariant**     |
+---------------------------------+--------------------+-------------------+
| ``Box<T>``                      |                    | **covariant**     |
+---------------------------------+--------------------+-------------------+
| ``Vec<T>``                      |                    | **covariant**     |
+---------------------------------+--------------------+-------------------+
| ``UnsafeCell<T>``               |                    | invariant         |
+---------------------------------+--------------------+-------------------+
| ``Cell<T>``                     |                    | invariant         |
+---------------------------------+--------------------+-------------------+
| ``PhantomData<T>``              |                    | **covariant**     |
+---------------------------------+--------------------+-------------------+
| ``Trait<T> + 'a``               | **covariant**      | invariant         |
+---------------------------------+--------------------+-------------------+

References
----------

- https://doc.rust-lang.org/nomicon/subtyping.html
- https://doc.rust-lang.org/reference/subtyping.html
