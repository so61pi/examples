#include <cstddef>
#include <iostream>
#include <string>
#include <tuple>
#include <type_traits>
#include <utility>


namespace detail {

    template<std::size_t Begin, std::size_t End>
    struct helper {
        template<typename Function, typename Tuple, typename... Args>
        decltype(auto) operator()(Function&& f, Tuple&& t, Args&&... args) const {
            return helper<Begin+1, End>{}(
                std::forward<Function>(f),
                std::forward<Tuple>(t),
                std::forward<Args>(args)...,
                std::get<Begin>(std::forward<Tuple>(t))
            );
        }
    };


    template<std::size_t End>
    struct helper<End, End> {
        template<typename Function, typename Tuple, typename... Args>
        decltype(auto) operator()(Function&& f, Tuple&&, Args&&... args) const {
            return f(std::forward<Args>(args)...);
        }
    };

} // namespace detail


template<typename Function, typename Tuple>
decltype(auto) expand(Function&& f, Tuple&& t) {
    return detail::helper<0, std::tuple_size<std::remove_reference_t<Tuple>>::value>{}(std::forward<Function>(f), std::forward<Tuple>(t));
}


void display(int a, char b, double c, std::string const& d) {
    std::cout << "display : " << a << " " << b << " " << c << " " << d << "\n";
}


class test_t {
public:
    static auto create() {
        return test_t{};
    }

    void display(int a, char b, double c, std::string& d) {
        std::cout << "test_t::display : " << a << " " << b << " " << c << " " << d << "\n";
    }

    void display(int a, char b, double c, std::string& d) const {
        std::cout << "test_t const::display : " << a << " " << b << " " << c << " " << d << "\n";
    }
};


int main() {
    auto t = std::make_tuple(1, '2', 3.0, std::string{ "4" });
    expand(display, t);

    test_t test{};
    expand([&test](auto&&... args) { test.display(std::forward<decltype(args)>(args)...); }, t);

    test_t const& ctest = test;
    expand([&ctest](auto&&... args) { ctest.display(std::forward<decltype(args)>(args)...); }, t);
}
