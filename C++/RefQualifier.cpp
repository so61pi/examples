#include <iostream>


struct test_t {
    void show() & { std::cout << "show - lvalue\n"; }
    void show() && { std::cout << "show - rvalue\n"; }
};


int main() {
    test_t test;
    test.show();
    test_t{}.show();
}
