#include <algorithm>
#include <functional>
#include <iostream>
#include <numeric>
#include <vector>


int main() {
    std::vector<int> v(10);

    for (decltype(v.size()) i = 0; i < v.size(); ++i) {
        v.at(i) = i + 1;
    }

    auto sum = std::accumulate(v.begin(), v.end(), 0);
    std::cout << "Sum: " << sum << "\n";

    auto product = std::accumulate(v.begin(), v.end(), 1, std::multiplies<int>());
    std::cout << "Product: " << product << "\n";
}
