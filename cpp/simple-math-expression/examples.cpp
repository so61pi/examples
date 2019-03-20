#include <cmath>
#include <iostream>
#include <string>
#include <vector>

#include "sme/sme.hpp"


template<std::size_t I, typename T>
bool is_greater_than(T const& t) {
    return t > I;
}


// define sme vars
SME_DEFINE_VAR(x, 0)
SME_DEFINE_VAR(y, 1)
SME_DEFINE_VAR(z, 2)

// turn normal functions to sme function
SME_DEFINE_FUNCTION(epow, std::pow)
SME_DEFINE_FUNCTION(esqrt, std::sqrt)
SME_DEFINE_FUNCTION(is_greater_than_1, is_greater_than<1>)


// return a sme function
auto create_function() {
    return esqrt(x * x + y * y) + epow(x, y) - is_greater_than_1(y);
}


int main() {
    std::cout << sme::evaluate((x + y + z)(x = 1, y = x + 2.0, z = y)) << "\n";

    // a same function can use with different types
    auto fxy = x + y;
    std::cout << sme::evaluate(fxy(x = 1, y = x + 2.0)) << "\n";
    std::cout << sme::evaluate(fxy(x = std::string{ "hello, " }, y = std::string{ "world" })) << "\n";

    auto fxyz = x[y + z];
    std::cout << sme::evaluate(fxyz(x = std::string{ "hello, world" }, y = 1, z = 2)) << "\n";
    std::cout << sme::evaluate(fxyz(x = std::vector<int>{ 1, 2, 3, 4, 5, 6 }, y = x[0], z = 2)) << "\n";

    // the result is fxy(x = 1, y = x + 2.0, y = 4) / fxy(x = 1, y = x + 2.0, x = 6)
    auto fx = fxy(y = 4);
    auto fy = fxy(x = 6);
    std::cout << sme::evaluate((fx / fy)(x = 1, y = x + 2.0)) << "\n";

    auto fxy2 = create_function();
    std::cout << sme::evaluate(fxy2((x = 3) & (y = 2))) << "\n";

    // scoped create a scope, any assignments in the new scope don't affect the outside scope
    auto fx2 = sme::scoped(x = 10) + x;
    std::cout << sme::evaluate(fx2(x = 1)) << "\n";

    // return z
    auto fxyz2 = (x, y, z);
    std::cout << sme::evaluate(fxyz2(x = 1, y = 2, z = 10)) << "\n\n";

    // return z
    auto fxyz3 = (x + y) ->* z;
    std::cout << sme::evaluate(fxyz3(x = 3, y = 2, z = 6)) << "\n";
}
