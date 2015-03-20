#include <iostream>
#include <random>


int main() {
    // create random device
    std::random_device rd;

    // create generator with random seed created from random device
    std::mt19937 gen(rd());

    // create a distribution in [1, 6] range
    std::uniform_int_distribution<> dis(1, 6);

    // generate a number in [1, 6] range randomly
    std::cout << dis(gen) << std::endl;

    // generate a number in [1, 10] range
    std::cout << dis(gen, { 1, 10 }) << std::endl;
}
