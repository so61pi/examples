#include <cstddef>
#include <iostream>


void f(bool) {}
void f(char) {}
void f(int) {}
void f(unsigned int) {}
void f(double) {}
void f(void *) {}
void f(int *) {}
void f(std::nullptr_t) {}


int main() {
    // std::cout << &f << "\n";
    std::cout << "Address of [void f(bool)] is "
        << static_cast<void(*)(bool)>(&f) << "\n";

    std::cout << "Address of [void f(char)] is "
        << static_cast<void(*)(char)>(&f) << "\n";

    std::cout << "Address of [void f(int)] is "
        << static_cast<void(*)(int)>(&f) << "\n";

    std::cout << "Address of [void f(unsigned int)] is "
        << static_cast<void(*)(unsigned int)>(&f) << "\n";

    std::cout << "Address of [void f(double)] is "
        << static_cast<void(*)(double)>(&f) << "\n";

    std::cout << "Address of [void f(void *)] is "
        << static_cast<void(*)(void *)>(&f) << "\n";

    std::cout << "Address of [void f(int *)] is "
        << static_cast<void(*)(int *)>(&f) << "\n";

    std::cout << "Address of [void f(std::nullptr_t)] is "
        << static_cast<void(*)(std::nullptr_t)>(&f) << "\n";
}
