#include <iostream>
#include <sstream>
#include <string>
#include <vector>


void f1();
void f2();
void WalkStack();


void f1() {
    volatile int i = -1;
    f2();
}


void f2() {
    volatile int i = -2;
    WalkStack();
}


void WalkStack() {
    volatile int i = -3;
    unsigned int place = 0;
    unsigned int *addr = &place;
    
    // walk stack
    std::vector<unsigned int> stack;
    for (int i = 0; i < 20; ++i) {
        stack.push_back(*addr);
        addr += 4;
    }

    for (auto& i : stack) {
        std::cout.width(8);
        std::cout << std::hex << i << std::endl;
    }
}


int main() {
    f1();
}
