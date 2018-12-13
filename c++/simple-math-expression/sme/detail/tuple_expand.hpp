#ifndef TUPLE_EXPAND_HPP
#define TUPLE_EXPAND_HPP


#include <cstddef>
#include <tuple>
#include <type_traits>
#include <utility>


namespace tuple_utility {


    namespace detail {


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


    } // namespace detail


    template<typename Tuple, typename Function>
    decltype(auto) expand(Tuple&& t, Function&& f) {
        return detail::helper<0, std::tuple_size<std::remove_reference_t<Tuple>>::value>{}(std::forward<Tuple>(t), std::forward<Function>(f));
    }


} // namespace tuple_utility


#endif // TUPLE_EXPAND_HPP
