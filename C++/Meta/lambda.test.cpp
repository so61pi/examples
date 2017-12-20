#include <iostream>
#include <type_traits>

#include "lambda.hpp"


int main() {
    lambda<
        std::common_type<
            std::common_type<_0, int, _2>,
            std::common_type<_1, long double>,
            _1
        >
    >::type::apply<bool, long, char>::type x;

    std::cout << std::is_same<decltype(x), long double>::value << "\n";
}
