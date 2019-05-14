pub fn fib(n: u8) -> u64 {
    if n < 2 {
        1
    } else {
        fib(n - 2) + fib(n - 1)
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_fib() {
        assert_eq!(fib(0), 1);
        assert_eq!(fib(1), 1);
        assert_eq!(fib(2), 2);
        assert_eq!(fib(3), 3);
        assert_eq!(fib(4), 5);
        assert_eq!(fib(5), 8);
        assert_eq!(fib(6), 13);
        assert_eq!(fib(7), 21);
        assert_eq!(fib(8), 34);
        assert_eq!(fib(9), 55);
    }
}
