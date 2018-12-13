#include <iostream>
#include <vector>
#include <boost/align.hpp>


int main() {
    // allocate memory with required alignment and size
    auto ptr = boost::alignment::aligned_alloc(
        16,     // required alignment, in bytes
        32);    // required size, in bytes
    if (ptr) {
        std::cout << "Allocated address: " << ptr << "\n";
        boost::alignment::aligned_free(ptr);
    }

    // create alignment vector (good to use with SSE)
    std::vector<int, boost::alignment::aligned_allocator<int, 16>> v;

    v.push_back(1);
    std::cout << "Address of underlying data: " << v.data() << "\n";

    v.push_back(1);
    v.push_back(1);
    v.push_back(1);
    std::cout << "Address of underlying data: " << v.data() << "\n";
}
