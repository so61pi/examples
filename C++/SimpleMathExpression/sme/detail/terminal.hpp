#ifndef TERMINAL_HPP
#define TERMINAL_HPP


#include <cstddef>
#include <type_traits>
#include <utility>
#include <boost/proto/proto.hpp>


namespace sme { namespace detail {


    namespace terminal {


        // define sme terminal id
        struct id {
            enum : std::size_t {
                unknown,
                var,
                original_function,
                scoped
            };
        };


        // get id of sme terminal
        template<typename T>
        struct sme_terminal_id {
            static std::size_t const value = id::unknown;
        };


        // macro to assign id to sme terminal
        #define DEFINE_SME_TERMINAL_ID(NAME, ID)                                                                            \
            template<typename T>                                                                                            \
            struct sme_terminal_id<NAME<T>> {                                                                               \
                static std::size_t const value = ID;                                                                        \
            };


        // var terminal
        template<typename I>
        struct var {};

        // original function terminal
        template<typename T>
        struct original_function {
            template<typename... Args>
            decltype(auto) operator()(Args&&... args) const {
                return T{}(std::forward<Args>(args)...);
            }
        };

        // scoped terminal
        template<typename Ignore = void>
        struct scoped {};


        //
        #define IS_SME_TERMINAL(NAME)                                                                                       \
            template<typename Var>                                                                                          \
            struct is_##NAME : std::false_type                                                                              \
            {};                                                                                                             \
                                                                                                                            \
            template<typename T>                                                                                            \
            struct is_##NAME<NAME<T>> : std::true_type                                                                      \
            {};                                                                                                             \
                                                                                                                            \
            template<typename T>                                                                                            \
            struct is_##NAME<NAME<T> const> : std::true_type                                                                \
            {};


        IS_SME_TERMINAL(var)
        IS_SME_TERMINAL(original_function)
        IS_SME_TERMINAL(scoped)

        DEFINE_SME_TERMINAL_ID(var, id::var)
        DEFINE_SME_TERMINAL_ID(original_function, id::original_function)
        DEFINE_SME_TERMINAL_ID(scoped, id::scoped)

        #undef DEFINE_SME_TERMINAL_ID
        #undef IS_SME_TERMINAL


        // get sme terminal id from terminal expr
        template<typename Expr>
        struct sme_terminal_expr_id {
            static std::size_t const value =
                sme_terminal_id<typename boost::proto::result_of::value<Expr>::type>::value;
        };


        // check if an expr is a sme terminal expr
        template<typename Expr>
        struct is_sme_terminal_expr {
            template<typename E, std::size_t>
            struct helper : std::false_type
            {};

            // proto terminals have arity of zero
            template<typename E>
            struct helper<E, 0> {
                static std::size_t const value = (sme_terminal_expr_id<E>::value != 0);
            };


            static std::size_t const value =
                helper<Expr, boost::proto::arity_of<Expr>::value>::value;
        };


    } // namespace terminal


}} // namespace sme::detail


#endif // TERMINAL_HPP
