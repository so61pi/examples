pub fn div(a: i32, b: i32) -> i32 {
    if b == 0 {
        panic!("Divide-by-zero error");
    }

    a / b
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_div() {
        assert_eq!(div(2, 1), 2);
    }

    #[test]
    #[should_panic]
    fn test_div_zero_1() {
        div(10, 0);
    }

    #[test]
    #[should_panic(expected = "Divide-by-zero error")]
    fn test_div_zero_2() {
        div(10, 0);
    }
}
