#include <iostream>


// enumeration using to choose which function will get called
// enum class is used to prevent user uses arbitrary integral constant to specify which function is called
// e.g. factorial<0>(5) is illegal
enum class FactorialMethod : size_t {
    Loop,
    Recursion
};


// default implementation...
template<FactorialMethod = FactorialMethod::Loop>
size_t factorial(size_t n) {
    size_t r = 1;
    for (size_t i = 1; i <= n; ++i)
        r *= i;
    return r;
}


// ... and another one
template<>
size_t factorial<FactorialMethod::Recursion>(size_t n) {
    if (n == 0)
        return 1;
    else
        return n * factorial<FactorialMethod::Recursion>(n - 1);
}


int main() {
    // call default implementation (use loop version implicitly)
    std::cout << factorial(5) << std::endl;
    std::cout << factorial<>(5) << std::endl;

    // use loop version explicitly
    std::cout << factorial<FactorialMethod::Loop>(5) << std::endl;

    // use recursive version
    std::cout << factorial<FactorialMethod::Recursion>(5) << std::endl;
}
