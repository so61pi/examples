#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <iterator>
#include <vector>


std::vector<char> ReadBytesFromFile(char const* const file, std::size_t size) {
    std::ifstream input(file, std::ios::binary);
    std::vector<char> result(size);
    std::istreambuf_iterator<char> it(input);
    std::copy_n(it, size, result.begin());
    return result;
}


std::vector<char> ReadRandomBytes(std::size_t size) {
    // Will not block if the extropy pool is exhausted.
    char const* const file = "/dev/urandom";
    return ReadBytesFromFile(file, size);
}


std::vector<char> ReadExtremeRandomBytes(std::size_t size) {
    // Will BLOCK if the extropy pool is exhausted.
    char const* const file = "/dev/random";
    return ReadBytesFromFile(file, size);
}


int main() {
    auto r1 = ReadRandomBytes(10);
    std::copy(r1.begin(), r1.end(), std::ostream_iterator<int>(std::cout, " "));
    std::cout << std::endl;

    auto r2 = ReadExtremeRandomBytes(10);
    std::copy(r2.begin(), r2.end(), std::ostream_iterator<int>(std::cout, " "));
    std::cout << std::endl;
}
