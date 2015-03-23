#include <algorithm>
#include <iostream>
#include <vector>
#include <immintrin.h>
#include <malloc.h>


template <typename T>
class my_allocator {
public:
    using value_type = T;


    T *allocate(std::size_t n) {
        // allocate 16-byte aligned memory
        return static_cast<T *>(_aligned_malloc(n * sizeof(T), 16));
    }


    void deallocate(T *p, std::size_t n) {
        _aligned_free(p);
    }
};


int main() {
    std::vector<int, my_allocator<int>> a(100, 2);
    std::vector<int, my_allocator<int>> b(100, 3);
    std::vector<int, my_allocator<int>> c(100);

    auto x = a.data();
    auto y = b.data();
    auto z = c.data();
    for (decltype(a.size()) i = 0; i < a.size() / 4; ++i) {
        *reinterpret_cast<__m128i *>(&z[i * 4]) = _mm_add_epi32(
            *reinterpret_cast<__m128i *>(&x[i * 4]),
            *reinterpret_cast<__m128i *>(&y[i * 4]));
    }

    for (const auto& i : c)
        std::cout << i << " ";

    std::cout << "\n";
}
