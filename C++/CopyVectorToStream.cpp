#include <algorithm>
#include <iostream>
#include <iterator>
#include <vector>


int main() {
    std::vector<int> v{ 1, 2, 3 };

    // print: "1 2 3 "
    std::copy(v.begin(), v.end(), std::ostream_iterator<int>(std::cout, " "));
}
