// Lookup Rules:
// - https://github.com/rust-lang/rustc-guide/blob/master/src/method-lookup.md
// - https://doc.rust-lang.org/reference/expressions/method-call-expr.html
//
// Simplified Rules:
// 1. Dereference to create a list of (self: T)
// 2. For each (self: U), find a match method in the following order
//    - (self: U)
//    - (self: &U)
//    - (self: &mut U)
// Note:
// - The described process stops on first match.
// - Inheret methods are prefered over trait methods.
//
// Example:
//      let o = &&Object;
//      o.name();
//
//      [(self: &&T), (self: &T), (self: T)]
//      with (self: &&T), find
//          name(self: &&T)
//          || name(self: &&&T)
//          || name(self: &mut &&T)
//      with (self: &T), find
//          name(self: &T)
//          || name(self: &&T)
//          || name(self: &mut &T)
//      with (self: T), find
//          name(self: T)
//          || name(self: &T)
//          || name(self: &mut T)

trait Trait {
    fn refer(&self) -> String;
    fn eatup(self) -> String;
}

struct TestStruct;

// 0
// Inheret methods (0) are prefered over trait methods (1).
impl TestStruct {
    // refer(self: &TestStruct)
    fn refer(&self) -> String {
        String::from("0.1-&TestStruct")
    }

    // eatup(self: TestStruct)
    fn eatup(self) -> String {
        String::from("0.2-TestStruct")
    }
}

// 1
impl Trait for TestStruct {
    // refer(self: &TestStruct)
    fn refer(&self) -> String {
        String::from("1.1-&TestStruct")
    }

    // eatup(self: TestStruct)
    fn eatup(self) -> String {
        String::from("1.2-TestStruct")
    }
}

// 2
impl Trait for &TestStruct {
    // refer(self: &&TestStruct)
    fn refer(&self) -> String {
        String::from("2.1-&&TestStruct")
    }

    // eatup(self: &TestStruct)
    fn eatup(self) -> String {
        String::from("2.2-&TestStruct")
    }
}

// 3
impl Trait for &mut TestStruct {
    // refer(self: &&mut TestStruct)
    fn refer(&self) -> String {
        String::from("3.1-&&mut TestStruct")
    }

    // eatup(self: &mut TestStruct)
    fn eatup(self) -> String {
        String::from("3.2-&mut TestStruct")
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_overload_1() {
        let o = TestStruct;
        assert_eq!(o.refer(), "0.1-&TestStruct");
        assert_eq!(o.eatup(), "0.2-TestStruct");

        let mut m = TestStruct;
        assert_eq!(m.refer(), "0.1-&TestStruct");
        assert_eq!(m.eatup(), "0.2-TestStruct");
    }

    #[test]
    fn test_overload_2() {
        let o = TestStruct;
        let ro = &o; // &TestStruct
        assert_eq!(ro.refer(), "0.1-&TestStruct");
        assert_eq!(ro.eatup(), "2.2-&TestStruct");
        // ro.refer()
        //      &TestStruct             refer(self: &TestStruct)            0.1 || 1.1
        //          &&TestStruct        refer(self: &&TestStruct)           2.1
        //          &mut &TestStruct    refer(self: &mut &TestStruct)       -
        //      TestStruct              refer(self: TestStruct)             -
        //          &TestStruct         refer(self: &TestStruct)            0.1 || 1.1
        //          &mut TestStruct     refer(self: &mut TestStruct)        -
        //      => first match is 0.1
        //
        // ro.eatup()
        //      &TestStruct             eatup(self: &TestStruct)            2.2
        //          &&TestStruct        eatup(self: &&TestStruct)           -
        //          &mut &TestStruct    eatup(self: &mut &TestStruct)       -
        //      TestStruct              eatup(self: TestStruct)             0.2 || 1.2
        //          &TestStruct         eatup(self: &TestStruct)            2.2
        //          &mut TestStruct     eatup(self: &mut TestStruct)        3.2
        //      => first match is 2.2

        let mut m = TestStruct;
        let rm = &mut m; // &mut TestStruct
        assert_eq!(rm.refer(), "3.1-&&mut TestStruct");
        assert_eq!(rm.eatup(), "3.2-&mut TestStruct");
        // rm.refer()
        //      &mut TestStruct             refer(self: &mut TestStruct)        -
        //          &&mut TestStruct        refer(self: &&mut TestStruct)       3.1
        //          &mut &mut TestStruct    refer(self: &mut &mut TestStruct)   -
        //      mut TestStruct              refer(self: mut TestStruct)         -
        //          &mut TestStruct         refer(self: &mut TestStruct)        -
        //          &mut TestStruct         refer(self: &mut TestStruct)        -
        //          ^~~~ one mut is omitted
        //      => first match is 3.1
        //
        // rm.eatup()
        //      &mut TestStruct             eatup(self: &mut TestStruct)        3.2
        //          &&mut TestStruct        eatup(self: &&mut TestStruct)       -
        //          &mut &mut TestStruct    eatup(self: &mut &mut TestStruct)   -
        //      mut TestStruct              eatup(self: mut TestStruct)         -
        //          &mut TestStruct         eatup(self: &mut TestStruct)        3.2
        //          &mut TestStruct         eatup(self: &mut TestStruct)        3.2
        //          ^~~~~~~~~~~~~~~ one mut is omitted
        //      => first match is 3.2
    }

    #[test]
    fn test_overload_3() {
        let o = TestStruct;
        let rro = &&o; // &&TestStruct
        assert_eq!(rro.refer(), "2.1-&&TestStruct");
        assert_eq!(rro.eatup(), "2.2-&TestStruct");

        let mut m = TestStruct;
        let rrm = &&mut m; // &&mut TestStruct
        assert_eq!(rrm.refer(), "3.1-&&mut TestStruct");
        // assert_eq!(rrm.eatup(), "&mut TestStruct");
    }
}
