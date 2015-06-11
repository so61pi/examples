#include <iostream>


void f() {
    std::cout << "f() returns void\n";
}


void g() {
    return f();
}


int main() {
    g();
}
