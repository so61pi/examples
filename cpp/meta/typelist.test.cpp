#include <iostream>

#include "typelist.hpp"


int main() {
    using namespace tl;
    
    using A = typelist<int, void *>;

    // B = typelist<int, void *, long>
    using B = push_back_t<A, long>;
    std::cout
        << std::boolalpha << std::is_same<B, typelist<int, void *, long>>::value
        << "\n";

    // C = typelist<double, int, void *, long>
    using C = push_front_t<B, double>;
    std::cout
        << std::is_same<C, typelist<double, int, void *, long>>::value
        << "\n";

    // D = typelist<double, int, void *, long, double, int, void *, long>
    using D = concat_t<C, C>;
    std::cout
        << std::is_same<D, typelist<double, int, void *, long, double, int, void *, long>>::value
        << "\n";

    // E = typelist<long, void *, int, double, long, void *, int, double>
    using E = reverse_t<D>;
    std::cout
        << std::is_same<E, typelist<long, void *, int, double, long, void *, int, double>>::value
        << "\n";

    // F = typelist<void *, int, double, long, void *, int, double>
    using F = pop_front_t<E>;
    std::cout
        << std::is_same<F, typelist<void *, int, double, long, void *, int, double>>::value
        << "\n";

    // G = typelist<void *, int, double, long, void *, int>
    using G = pop_back_t<F>;
    std::cout
        << std::is_same<G, typelist<void *, int, double, long, void *, int>>::value
        << "\n";

    // H = void *
    using H = at_c_t<G, 0>;
    std::cout
        << std::is_same<H, void *>::value
        << "\n";

    // I = void *
    using I = front_t<G>;
    std::cout
        << std::is_same<H, I>::value
        << "\n";
    
    // J = typelist<void *, int, double, long, void *, int, char *>
    using J = insert_c_t<G, char *, 6>;
    std::cout
        << std::is_same<J, typelist<void *, int, double, long, void *, int, char *>>::value
        << "\n";
    
    // K = typelist<void *, int, double, long, void *, int>
    using K = remove_c_t<J, 6>;
    std::cout
        << std::is_same<K, typelist<void *, int, double, long, void *, int>>::value
        << "\n";
}
