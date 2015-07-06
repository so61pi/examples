#include <array>
#include <iostream>


template<typename T>
struct array_size;

template<typename T, std::size_t N>
struct array_size<T[N]> {
    static std::size_t const value = N;
    static std::size_t const bytes = N * sizeof(T);
};

// specialization for std::array
template<typename T, std::size_t N>
struct array_size<std::array<T, N>> {
    static std::size_t const value = N;
    static std::size_t const bytes = N * sizeof(T);
};


int main() {
    double a[10];
    std::cout << array_size<decltype(a)>::value << "\n";
    std::cout << array_size<decltype(a)>::bytes << "\n";
    
    std::array<short, 5> b;
    std::cout << array_size<decltype(b)>::value << "\n";
    std::cout << array_size<decltype(b)>::bytes << "\n";
}
