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


    template<typename Object, typename MemFun>
    struct memfun_wrapper {
        static_assert(std::is_reference<Object>::value, "object type must be reference type");
        static_assert(std::is_member_function_pointer<MemFun>::value, "member function must be a pointer type");

        memfun_wrapper(Object obj, MemFun func) :
            m_obj{ std::forward<Object>(obj) },
            m_func{ func }
        {}

        template<typename... Args>
        decltype(auto) operator()(Args&&... args) const {
            // if you get an error about const qualifier here
            // then the constness of m_func is not compatible with m_obj
            return (m_obj.*m_func)(std::forward<Args>(args)...);
        }

        Object m_obj;
        MemFun m_func;
    };

} // namespace detail


template<typename Function, typename Tuple>
decltype(auto) expand(Function&& f, Tuple&& t) {
    return detail::helper<0, std::tuple_size<std::remove_reference_t<Tuple>>::value>{}(std::forward<Function>(f), std::forward<Tuple>(t));
}


template<typename Object, typename MemFun, typename Tuple>
decltype(auto) expand(Object&& obj, MemFun memfun, Tuple&& t) {
    detail::memfun_wrapper<Object&&, MemFun> f{ std::forward<Object>(obj), memfun };
    return expand(f, std::forward<Tuple>(t));
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
};


int main() {
    auto t = std::make_tuple(1, '2', 3.0, std::string{ "4" });
    expand(display, t);

    test_t test{};
    expand([&test](auto&&... args) { test.display(args...); }, t);
    expand(test, &test_t::display, t);
    expand(test_t::create(), &test_t::display, t);
}
