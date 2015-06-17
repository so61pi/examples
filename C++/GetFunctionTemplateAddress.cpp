#include <iostream>


template<typename T>
void dwit() {
}


int main() {
    // get address of a function template instance make it instantiates
    std::cout << (void *)&dwit<int> << "\n";
    std::cout << (void *)&dwit<double> << "\n";
}
