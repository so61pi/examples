#include <iostream>


struct loop_tag_t {} loop_tag;
struct recursive_tag_t {} recursive_tag;


size_t factorial(size_t n, loop_tag_t) {
    size_t r = 1;
    for (size_t i = 1; i <= n; ++i)
        r *= i;
    return r;
}


size_t factorial(size_t n, recursive_tag_t) {
    if (n == 0)
        return 1;
    else
        return n * factorial(n - 1, recursive_tag);
}


int main() {
    std::cout << factorial(5, loop_tag) << std::endl;

    std::cout << factorial(5, recursive_tag) << std::endl;
}
