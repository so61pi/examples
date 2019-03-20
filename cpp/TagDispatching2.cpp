#include <cstddef>
#include <iostream>


namespace tag {
    struct is_positive;
    struct is_negative;
    struct is_zero;
}


template<int N, typename PredTag>
struct test;

template<int N>
struct test < N, tag::is_positive > {
    static const std::size_t value = (N > 0) ? 1 : 0;
};

template<int N>
struct test < N, tag::is_negative > {
    static const std::size_t value = (N < 0) ? 1 : 0;
};

template<int N>
struct test < N, tag::is_zero > {
    static const std::size_t value = (N == 0) ? 1 : 0;
};


int main() {
    std::cout
        << test<0, tag::is_zero>::value << "\n";

    std::cout
        << test<1, tag::is_negative>::value << "\n";

    std::cout
        << test<2, tag::is_positive>::value << "\n";
}
