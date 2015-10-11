#include <cstddef>
#include <iostream>
#include <tuple>
#include <type_traits>
#include <utility>


class tuple_printer {
    using this_type = tuple_printer;


    template<std::size_t Index, std::size_t PrevEnd, typename Ignore = void>
    struct helper {
        template<typename Tuple>
        void operator()(this_type const& outer, std::ostream& os, Tuple const& tuple) const {
            os << std::get<Index>(tuple) << outer.m_separate_str;
            helper<Index + 1, PrevEnd>{}(outer, os, tuple);
        }
    };

    template<std::size_t PrevEnd, typename Ignore>
    struct helper<PrevEnd, PrevEnd, Ignore> {
        template<typename Tuple>
        void operator()(this_type const&, std::ostream& os, Tuple const& tuple) const {
            os << std::get<PrevEnd>(tuple);
        }
    };


public:
    tuple_printer() = default;
    tuple_printer(char const* open_str, char const* close_str, char const* separate_str) :
        m_open_str{ open_str },
        m_close_str{ close_str },
        m_separate_str{ separate_str }
    {}


    template<typename Tuple, std::enable_if_t<std::tuple_size<Tuple>::value != 0> * = nullptr>
    void operator()(std::ostream& os, Tuple const& tuple) const {
        os << m_open_str;
        helper<0, std::tuple_size<Tuple>::value - 1>{}(*this, os, tuple);
        os << m_close_str;
    }


    template<typename Tuple, std::enable_if_t<std::tuple_size<Tuple>::value == 0> * = nullptr>
    void operator()(std::ostream& os, Tuple const& tuple) const {
        os << m_open_str << m_close_str;
    }


private:
    char const *const m_open_str = "[";
    char const *const m_close_str = "]";
    char const *const m_separate_str = ", ";
};


template<typename... Ts>
std::ostream& operator<<(std::ostream& os, std::tuple<Ts...> const& tuple) {
    tuple_printer{}(os, tuple);
    return os;
}


int main() {
    auto tuple = std::make_tuple(1, '2', 3.1);
    std::cout << tuple;
}
