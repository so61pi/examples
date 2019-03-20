#include <iostream>
#include <immintrin.h>


// this program shows how to add 2 arrays using SSE intrinsic functions
int main() {
    alignas(16) int a[4]{1, 2, 3, 4};
    alignas(16) int b[4]{10, 20, 30, 40};
    __m128i x{};
    __m128i y{};
    __m128i z{};

    // set value from high order int to low order int
    // (from a[3] to a[0])
    x = _mm_set_epi32(a[3], a[2], a[1], a[0]);
    y = _mm_set_epi32(b[3], b[2], b[1], b[0]);
    z = _mm_add_epi32(x, y);
    std::cout << z.m128i_i32[0] << " "
        << z.m128i_i32[1] << " "
        << z.m128i_i32[2] << " "
        << z.m128i_i32[3] << "\n";

    // set value from low order int to high order int
    // (from a[0] to a[3])
    x = _mm_setr_epi32(a[0], a[1], a[2], a[3]);
    y = _mm_setr_epi32(b[0], b[1], b[2], b[3]);
    z = _mm_add_epi32(x, y);
    std::cout << z.m128i_i32[0] << " "
        << z.m128i_i32[1] << " "
        << z.m128i_i32[2] << " "
        << z.m128i_i32[3] << "\n";

    // this way generates much less code
    z = _mm_add_epi32(
        *reinterpret_cast<__m128i *>(a),
        *reinterpret_cast<__m128i *>(b));
    std::cout << z.m128i_i32[0] << " "
        << z.m128i_i32[1] << " "
        << z.m128i_i32[2] << " "
        << z.m128i_i32[3] << "\n";
}
