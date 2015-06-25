#include <cstddef>
#include <iostream>
#include <string>
#include <tuple>
#include <type_traits>
#include <utility>


template<std::size_t Begin, std::size_t End>
struct helper {
    template<typename Tuple, typename Function, typename... Args>
    decltype(auto) operator()(Tuple&& t, Function&& f, Args&&... args) const {
        return helper<Begin+1, End>{}(
            std::forward<Tuple>(t),
            std::forward<Function>(f),
            std::forward<Args>(args)...,
            std::get<Begin>(std::forward<Tuple>(t))
        );
    }
};


template<std::size_t End>
struct helper<End, End> {
    template<typename Tuple, typename Function, typename... Args>
    decltype(auto) operator()(Tuple&& t, Function&& f, Args&&... args) const {
        return f(std::forward<Args>(args)...);
    }
};


template<typename Tuple, typename Function>
decltype(auto) expand(Tuple&& t, Function&& f) {
    return helper<0, std::tuple_size<std::remove_reference_t<Tuple>>::value>{}(std::forward<Tuple>(t), std::forward<Function>(f));
}


void display(int a, char b, double c, std::string const& d) {
    std::cout << a << "\n" << b << "\n" << c << "\n" << d << "\n";
}


int main() {
    auto t = std::make_tuple(1, '2', 3.0, std::string{ "4" });
    expand(t, display);
}
