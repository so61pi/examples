#include <iostream>


struct Base {};
struct Derived : Base {};


void foo(Base *) {
    std::cout << "foo(Base *)\n";
}


void foo(Derived *) {
    std::cout << "foo(Derived *)\n";
}


int main() {
    Derived *pd{};
    Base *pb{};

    // call best-match function
    foo(pd); // call foo(Derived *)
    foo(pb); // call foo(Base *)
}
