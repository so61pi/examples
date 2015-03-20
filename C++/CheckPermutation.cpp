#include <algorithm>
#include <iostream>
#include <vector>


int main() {
    std::vector<int> v1{ 1, 2, 3 };
    std::vector<int> v2{ 3, 1, 2 };

    // check if v1 is a permutation of v2
    // return true
    std::cout << std::is_permutation(v1.begin(), v1.end(), v2.begin(), v2.end())
        << std::endl;

    // v2 = { 3, 1, 2, 9 }
    v2.push_back(9);

    // check range [v1.begin(), v1.end())
    // in [v2.begin(), v2.begin() + (v1.end() - v1.begin()))
    // in this case, the fourth element of v2 (9) is ignored
    // return true
    std::cout << std::is_permutation(v1.begin(), v1.end(), v2.begin())
        << std::endl;

    // v1.size() != v2.size()
    // return false
    std::cout << std::is_permutation(v1.begin(), v1.end(), v2.begin(), v2.end())
        << std::endl;
}
