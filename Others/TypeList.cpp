#include <cstddef>
#include <iostream>
#include <type_traits>


struct end_type;


// typelist definition
template<typename... Types>
struct typelist;

template<typename First, typename... Lasts>
struct typelist < First, Lasts... > {
    using first_type = First;
    using next = typelist < Lasts... > ;

    using size = std::integral_constant < std::size_t, next::size::value + 1 > ;
};

template<>
struct typelist < > {
    using first_type = end_type;

    using size = std::integral_constant < std::size_t, 0 > ;
};


// push front a new type
template<typename First, typename TypeList>
struct push_front;

template<typename First, typename... Lasts>
struct push_front < typelist<Lasts...>, First > {
    using type = typelist < First, Lasts... > ;
};


// push back a new type
template<typename TypeList, typename T>
struct push_back {
private:
    using sub = typename push_back<typename TypeList::next, T>::type;

public:
    using type = typename push_front<sub, typename TypeList::first_type>::type;
};

template<typename T>
struct push_back < typelist<>, T > {
    using type = typelist < T > ;
};


// reverse
//
// get reversed list of sub list, then push current type to the back
template<typename TypeList>
struct reverse {
private:
    using reversed_sub = typename reverse<typename TypeList::next>::type;

public:
    using type = typename push_back <reversed_sub, typename TypeList::first_type>::type;
};

template<>
struct reverse < typelist<> > {
    using type = typelist < > ;
};


// pop front
template<typename TypeList>
struct pop_front {
    using type = typename TypeList::next;
};


// pop back
template<typename TypeList>
struct pop_back {
private:
    using temp = typename pop_front<typename reverse<TypeList>::type>::type;

public:
    using type = typename reverse<temp>::type;
};


// concatenate 2 typelists
template<typename Left, typename Right>
struct concat {
    using newLeft = typename push_back<Left, typename Right::first_type>::type;
    using newRight = typename Right::next;

    using type = typename concat<newLeft, newRight>::type;
};

template<typename Left>
struct concat < Left, typelist<> > {
    using type = Left;
};


// index must be in range [0; TypeList::size]
template<typename TypeList, std::size_t Index>
struct at {
    static_assert(Index <= TypeList::size::value, "Index out of range.");

    using type = typename at<typename TypeList::next, Index - 1>::type;
};

template<typename TypeList>
struct at < TypeList, 0 > {
    using type = typename TypeList::first_type;
};


int main() {
    using A = typelist < int, void * > ;

    // B = typelist < int, void *, long >
    using B = push_back < A, long >::type;
    std::cout
        << std::boolalpha << std::is_same<B, typelist<int, void *, long>>::value
        << "\n";

    // C = typelist < double, int, void *, long >
    using C = push_front < B, double >::type;
    std::cout
        << std::is_same<C, typelist<double, int, void *, long>>::value
        << "\n";

    // D = typelist < double, int, void *, long, double, int, void *, long >
    using D = concat < C, C >::type;
    std::cout
        << std::is_same<D, typelist<double, int, void *, long, double, int, void *, long>>::value
        << "\n";

    // E = typelist < long, void *, int, double, long, void *, int, double >
    using E = reverse<D>::type;
    std::cout
        << std::is_same<E, typelist<long, void *, int, double, long, void *, int, double>>::value
        << "\n";

    // F = typelist < void *, int, double, long, void *, int, double >
    using F = pop_front<E>::type;
    std::cout
        << std::is_same<F, typelist<void *, int, double, long, void *, int, double>>::value
        << "\n";

    // G = typelist < void *, int, double, long, void *, int >
    using G = pop_back<F>::type;
    std::cout
        << std::is_same<G, typelist<void *, int, double, long, void *, int>>::value
        << "\n";

    // H = void *
    using H = at<G, 0>::type;
    std::cout
        << std::is_same<H, void *>::value
        << "\n";
}
