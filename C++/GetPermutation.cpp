#include <algorithm>
#include <iostream>
#include <vector>


int main() {
    std::vector<int> v{ 3, 2, 3, 1 };

    // v = { 1, 2, 3, 3 }
    std::sort(v.begin(), v.end());

    // print all permutations of { 1, 2, 3, 3 } including itself
    // in ascending lexicographical order
    do {
        for (auto& i : v)
            std::cout << i << " ";
        std::cout << std::endl;
    } while (std::next_permutation(v.begin(), v.end()));

    std::cout << std::endl;
}
