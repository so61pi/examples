#include <algorithm>
#include <iostream>
#include <iterator>
#include <vector>


int main() {
    std::vector<int> v1{ 1, 2, 3 };
    std::vector<int> v2{ 4, 5, 6 };

    // concatenate 2 vectors
    std::copy(v2.begin(), v2.end(), std::back_inserter(v1));

    // print: "1 2 3 4 5 6 "
    for (auto& i : v1)
        std::cout << i << " ";

    std::cout << std::endl;
}
