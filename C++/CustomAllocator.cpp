#include <iostream>
#include <stdexcept>
#include <vector>


template <typename T>
class my_allocator {
public:
    using value_type = T;


    T *allocate(std::size_t n) {
        std::cout << "allocate\n";
        T *p = static_cast<T *>(::operator new(n * sizeof(T), std::nothrow));
        if (p == nullptr) {
            throw std::bad_alloc();
        }
        return p;
    }


    void deallocate(T *p, std::size_t n) {
        std::cout << "deallocate\n";
        ::operator delete(p);
    }
};


int main() {
    std::vector<int, my_allocator<int>> v{ 1, 2, 3, 4 };
    for (const auto& i : v)
        std::cout << i << " ";
    std::cout << "\n";
}
