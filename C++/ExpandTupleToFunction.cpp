#include <iostream>
#include <string>
#include <tuple>
#include <type_traits>
#include <utility>


template<typename F, typename Tuple, std::size_t... I>
auto expand(F&& f, Tuple&& args, std::index_sequence<I...>) -> decltype(auto) {
    return std::forward<F>(f)(std::get<I>(std::forward<Tuple>(args))...);
}


template<typename F, typename Tuple>
auto expand(F&& f, Tuple&& args) -> decltype(auto) {
    return expand(std::forward<F>(f), std::forward<Tuple>(args), std::make_index_sequence<std::tuple_size<std::remove_reference_t<Tuple>>::value>{});
}


void foo(int a, double b, std::string const& c) {
    std::cout << a << ' ' << b << ' ' << c << '\n';
}


void bar(int a, double b) {
    std::cout << a << ' ' << b << '\n';
}


int main() {
    using namespace std::literals;
    auto tuple = std::make_tuple(1, 2.0, "abc"s);
    expand(foo, tuple);
    expand(bar, tuple, std::index_sequence<0, 1>{});
}
