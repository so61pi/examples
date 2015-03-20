#include <algorithm>
#include <iostream>
#include <vector>
#include <immintrin.h>


int main() {
    std::vector<int> a(100, 2);
    std::vector<int> b(100, 3);
    std::vector<int> c(100);

    auto x = a.data();
    auto y = b.data();
    auto z = c.data();
    for (int i = 0; i < a.size() / 4; ++i) {
        *reinterpret_cast<__m128i *>(&z[i * 4]) = _mm_add_epi32(
            *reinterpret_cast<__m128i *>(&a[i * 4]),
            *reinterpret_cast<__m128i *>(&b[i * 4]));
    }

    for (const auto& i : c)
        std::cout << i << " ";

    std::cout << "\n";
}
