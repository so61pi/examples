#include <iostream>
#include <limits>
#include <type_traits>


namespace node {
    struct end_type;


    // node definition
    template<typename... Types>
    struct node;

    template<typename First, typename... Lasts>
    struct node < First, Lasts... > {
        using first_type = First;
        using next_node = node < Lasts... > ;

        using size = std::integral_constant < std::size_t, next_node::size::value + 1 > ;
    };

    template<>
    struct node < > {
        using first_type = end_type;

        using size = std::integral_constant < std::size_t, 0 > ;
    };


    // push front a new type
    template<typename First, typename Node>
    struct push_front;

    template<typename First, typename... Lasts>
    struct push_front < node<Lasts...>, First > {
        using type = node < First, Lasts... > ;
    };


    // push back a new type
    template<typename Node, typename T>
    struct push_back {
        using x = typename Node::first_type;
        using y = typename push_back<typename Node::next_node, T>::type;

        using type = typename push_front<y, x>::type;
    };

    template<typename T>
    struct push_back < node<>, T > {
        using type = node < T > ;
    };


    // reverse
    template<typename Node>
    struct reverse {
        using x = typename Node::first_type;
        using y = typename reverse<typename Node::next_node>::type;

        using type = typename push_back <y, x>::type;
    };

    template<>
    struct reverse < node<> > {
        using type = node < > ;
    };


    // concatenate 2 nodes
    template<typename Left, typename Right>
    struct concat {
        using newLeft = typename push_back<Left, typename Right::first_type>::type;
        using newRight = typename Right::next_node;

        using type = typename concat<newLeft, newRight>::type;
    };

    template<typename Left>
    struct concat < Left, node<> > {
        using type = Left;
    };


    // index must be in range [0; Node::size]
    template<typename Node, std::size_t Index>
    struct at {
        static_assert(Index <= Node::size::value, "Index out of range.");

        using type = typename at<typename Node::next_node, Index - 1>::type;
    };

    template<typename Node>
    struct at < Node, 0 > {
        using type = typename Node::first_type;
    };
}


int main() {
    using A = node::node < int, void * > ;

    // B = node < int, void *, long >
    using B = node::push_back < A, long >::type;
    std::cout
        << std::boolalpha << std::is_same<B, node::node<int, void *, long>>::value
        << "\n";

    // C = node < double, int, void *, long >
    using C = node::push_front < B, double >::type;
    std::cout
        << std::is_same<C, node::node<double, int, void *, long>>::value
        << "\n";

    // D = node < double, int, void *, long, double, int, void *, long >
    using D = node::concat < C, C >::type;
    std::cout
        << std::is_same<D, node::node<double, int, void *, long, double, int, void *, long>>::value
        << "\n";

    // E = node < long, void *, int, double, long, void *, int, double >
    using E = node::reverse<D>::type;
    std::cout
        << std::is_same<E, node::node<long, void *, int, double, long, void *, int, double>>::value
        << "\n";
}
