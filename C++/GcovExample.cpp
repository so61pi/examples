#include <iostream>
#include <numeric>
#include <vector>


/// g++ --coverage -O0 main.cpp
/// ./a.out
/// lcov --capture --directory . --output-file coverage.info
/// genhtml coverage.info --output-directory html


int main() {
    std::vector<int> v(1000);
    std::iota(v.begin(), v.end(), 1);

    int sum = 0;
    for (auto const& e : v) {
        int i = 4;
        switch (e % 3) {
        case 0:
            i = e;
            break;

        case 1:
            i = e - 1;
            break;

        case 2:
            i = e + 1;
            break;
        }

        sum += i;
    }

    if (sum % 2) {
        std::cout << "sum is odd\n";
    } else {
        std::cout << "sum is even\n";
    }
}
