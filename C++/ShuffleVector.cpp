#include <algorithm>
#include <iostream>
#include <random>
#include <vector>


int main() {
    std::vector<int> v{ 1, 2, 3, 4, 5, 6 };

    // shuffle using a RNG
    std::random_device rd;
    std::mt19937 gen(rd());
    std::shuffle(v.begin(), v.end(), gen);

    for (auto& i : v)
        std::cout << i << " ";
    
    std::cout << std::endl;
}
