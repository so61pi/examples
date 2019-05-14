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
