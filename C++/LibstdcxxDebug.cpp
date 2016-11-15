#include <iostream>
#include <vector>

#include <debug/vector>


/// Read more at
///     https://gcc.gnu.org/onlinedocs/libstdc++/manual/debug_mode.html


int main() {
    /// For this case, use
    ///     g++ -g3 -D_GLIBCXX_DEBUG main.cpp
    std::vector<int> a(5);
    std::cout << a[5] << std::endl;

    /// For this case, use
    ///     g++ -g3 main.cpp
    __gnu_debug::vector<int> b(5);
    std::cout << b[5] << std::endl;
}
