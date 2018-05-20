#include <iostream>

#include <experimental/coroutine>

#include "generator.h"

// Compile with
//   clang++ -fcoroutines-ts -stdlib=libc++ main.cpp


generator<int> fib(unsigned n) {
    unsigned a = 0;
    unsigned b = 1;

    for (int i = 0; i < n; ++i) {
        co_yield a;
        a += b;
        std::swap(a, b);
    }
}


int main() {
    for (auto const& x : fib(10)) {
        std::cout << x << ' ';
    }
    std::cout << std::endl;
}
