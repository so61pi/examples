#ifndef EVALUATE_HPP
#define EVALUATE_HPP


#include <cstddef>
#include <tuple>
#include <type_traits>
#include <utility>
#include <boost/proto/proto.hpp>

#include "domain.hpp"
#include "terminal.hpp"
#include "tuple_expand.hpp"
#include "var.hpp"
#include "varstore.hpp"


namespace sme { namespace detail {


        namespace proto = boost::proto;


        struct eval_result {
            // create eval result
            template<typename Value, typename VarStore>
            static auto create(Value&& value, VarStore&& varstore) {
                return std::make_tuple(std::forward<Value>(value), std::forward<VarStore>(varstore));
            }

            // get value from eval result
            template<typename EvalResult>
            static decltype(auto) get_value(EvalResult const& result) {
                return std::get<0>(result);
            }

            // get varstore from eval result
            template<typename EvalResult>
            static decltype(auto) get_varstore(EvalResult const& result) {
                return std::get<1>(result);
            }
        };


        //
        template<
            typename Expr,
            typename Tag = typename proto::tag_of<Expr>::type
        >
        struct eval;


        // evaluate an expr with given varstore
        template<typename Expr, typename VarStore>
        auto evaluate(Expr const& expr, VarStore const& varstore) {
            eval<Expr const> eval_function;
            return eval_function(expr, varstore);
        }


        // +x, -x, !x ...
        #define UNARY_OPERATOR(OP, TAG)                                                                                     \
            template<typename Expr>                                                                                         \
            struct eval<Expr, proto::tag::TAG> {                                                                            \
                template<typename VarStore>                                                                                 \
                auto operator()(Expr const& expr, VarStore const& varstore) const {                                         \
                    auto x = evaluate(proto::child(expr), varstore);                                                        \
                    return eval_result::create( OP eval_result::get_value(x), eval_result::get_varstore(x));                \
                }                                                                                                           \
            };


        // x + y, x - y ...
        #define BINARY_OPERATOR(OP, TAG)                                                                                    \
            template<typename Expr>                                                                                         \
            struct eval<Expr, proto::tag::TAG> {                                                                            \
                template<typename VarStore>                                                                                 \
                auto operator()(Expr const& expr, VarStore const& varstore) const {                                         \
                    auto computed_lhs = evaluate(proto::left(expr), varstore);                                              \
                    auto computed_rhs = evaluate(                                                                           \
                        proto::right(expr),                                                                                 \
                        varstore::create_view(eval_result::get_varstore(computed_lhs), varstore)                            \
                    );                                                                                                      \
                                                                                                                            \
                    auto new_varstore = varstore::create_view(                                                              \
                        eval_result::get_varstore(computed_rhs),                                                            \
                        eval_result::get_varstore(computed_lhs)                                                             \
                    );                                                                                                      \
                                                                                                                            \
                    return eval_result::create(                                                                             \
                        eval_result::get_value(computed_lhs) OP eval_result::get_value(computed_rhs),                       \
                        varstore::varstore_from_view(new_varstore)                                                          \
                    );                                                                                                      \
                }                                                                                                           \
            };


        // x += y ...
        #define COMPOUND_ASSIGNMENT_OPERATOR(OP, TAG)                                                                       \
            template<typename Expr>                                                                                         \
            struct eval<Expr, proto::tag::TAG> {                                                                            \
                template<typename VarStore>                                                                                 \
                auto operator()(Expr const& expr, VarStore const& varstore) const {                                         \
                    using VarId =                                                                                           \
                        typename proto::result_of::value<                                                                   \
                            typename proto::result_of::left<Expr const>::type                                               \
                        >::type;                                                                                            \
                    static_assert(                                                                                          \
                        terminal::is_var<VarId>::value,                                                                     \
                        "\n\n\n\n*****     left hand side of an assignment must be a var     *****\n\n\n"                   \
                    );                                                                                                      \
                                                                                                                            \
                    auto computed_rhs = evaluate(proto::right(expr), varstore);                                             \
                    auto lhs = var::get_value(                                                                              \
                        varstore::get_var<VarId>(                                                                           \
                            varstore::create_view(eval_result::get_varstore(computed_rhs), varstore)                        \
                        )                                                                                                   \
                    );                                                                                                      \
                                                                                                                            \
                    lhs OP eval_result::get_value(computed_rhs);                                                            \
                                                                                                                            \
                    auto new_var = varstore::varstore_from_var(var::create_var<VarId>(lhs));                                \
                    auto new_varstore = varstore::create_view(                                                              \
                        new_var,                                                                                            \
                        eval_result::get_varstore(computed_rhs)                                                             \
                    );                                                                                                      \
                                                                                                                            \
                    return eval_result::create(lhs, varstore::varstore_from_view(new_varstore));                            \
                }                                                                                                           \
            };


        // !sizeof(Expr) is always false
        // it's there just to make sure the compiler doesn't assert until this struct actually instantiated
        #define NOT_SUPPORTED_OPERATOR(OP, TAG)                                                                             \
            template<typename Expr>                                                                                         \
            struct eval<Expr, proto::tag::TAG> {                                                                            \
                static_assert(                                                                                              \
                    !sizeof(Expr),                                                                                          \
                    "\n\n\n\n*****     " #OP " operator is not supported      *****\n\n\n"                                  \
                );                                                                                                          \
            };


        UNARY_OPERATOR(+, unary_plus)
        UNARY_OPERATOR(-, negate)
        UNARY_OPERATOR(~, complement)
        UNARY_OPERATOR(!, logical_not)
        UNARY_OPERATOR(++, pre_inc)
        UNARY_OPERATOR(--, pre_dec)
        NOT_SUPPORTED_OPERATOR(*, dereference)
        NOT_SUPPORTED_OPERATOR(&, address_of)
        NOT_SUPPORTED_OPERATOR(++, post_inc)
        NOT_SUPPORTED_OPERATOR(--, post_dec)

        BINARY_OPERATOR(<<, shift_left)
        BINARY_OPERATOR(>>, shift_right)
        BINARY_OPERATOR(*, multiplies)
        BINARY_OPERATOR(/, divides)
        BINARY_OPERATOR(%, modulus)
        BINARY_OPERATOR(+, plus)
        BINARY_OPERATOR(-, minus)

        BINARY_OPERATOR(<, less)
        BINARY_OPERATOR(>, greater)
        BINARY_OPERATOR(<=, less_equal)
        BINARY_OPERATOR(>=, greater_equal)
        BINARY_OPERATOR(==, equal_to)
        BINARY_OPERATOR(!=, not_equal_to)

        // cannot implement short circuit evaluation
        BINARY_OPERATOR(||, logical_or)
        BINARY_OPERATOR(&&, logical_and)

        BINARY_OPERATOR(&, bitwise_and)
        BINARY_OPERATOR(|, bitwise_or)
        BINARY_OPERATOR(^, bitwise_xor)

        COMPOUND_ASSIGNMENT_OPERATOR(<<=, shift_left_assign)
        COMPOUND_ASSIGNMENT_OPERATOR(>>=, shift_right_assign)
        COMPOUND_ASSIGNMENT_OPERATOR(*=, multiplies_assign)
        COMPOUND_ASSIGNMENT_OPERATOR(/=, divides_assign)
        COMPOUND_ASSIGNMENT_OPERATOR(%=, modulus_assign)
        COMPOUND_ASSIGNMENT_OPERATOR(+=, plus_assign)
        COMPOUND_ASSIGNMENT_OPERATOR(-=, minus_assign)
        COMPOUND_ASSIGNMENT_OPERATOR(&=, bitwise_and_assign)
        COMPOUND_ASSIGNMENT_OPERATOR(|=, bitwise_or_assign)
        COMPOUND_ASSIGNMENT_OPERATOR(^=, bitwise_xor_assign)


        #undef UNARY_OPERATOR
        #undef BINARY_OPERATOR
        #undef COMPOUND_ASSIGNMENT_OPERATOR
        #undef NOT_SUPPORTED_OPERATOR


        //
        // terminal
        //
        template<typename Expr>
        struct eval<Expr, proto::tag::terminal> {
            // constant terminal
            template<typename Terminal>
            struct helper {
                template<typename VarStore>
                auto operator()(Expr const& expr, VarStore const& varstore) const {
                    return eval_result::create(proto::value(expr), varstore::empty());
                }
            };


            // var terminal
            template<typename I>
            struct helper<terminal::var<I>> {
                template<typename VarStore>
                auto operator()(Expr const& expr, VarStore const& varstore) const {
                    return eval_result::create(
                        var::get_value(varstore::get_var<terminal::var<I>>(varstore)),
                        varstore::empty()
                    );
                }
            };


            template<typename VarStore>
            auto operator()(Expr const& expr, VarStore const& varstore) const {
                using Terminal = typename proto::result_of::value<Expr const>::type;
                return helper<Terminal>{}(expr, varstore);
            }
        };


        //
        // x = y
        //
        template<typename Expr>
        struct eval<Expr, proto::tag::assign> {
            template<typename VarStore>
            auto operator()(Expr const& expr, VarStore const& varstore) const {
                // left hand side must be a var
                using VarId =
                    typename proto::result_of::value<
                        typename proto::result_of::left<Expr const>::type
                    >::type;
                static_assert(
                    terminal::is_var<VarId>::value,
                    "\n\n\n\n*****     left hand side of an assignment must be a var     *****\n\n\n"
                );

                auto computed_rhs = evaluate(proto::right(expr), varstore);
                auto new_var = varstore::varstore_from_var(var::create_var<VarId>(eval_result::get_value(computed_rhs)));

                auto new_varstore = varstore::create_view(
                    new_var,
                    eval_result::get_varstore(computed_rhs)
                );

                return eval_result::create(
                    eval_result::get_value(computed_rhs),
                    varstore::varstore_from_view(new_varstore)
                );
            }
        };


        //
        // x(y, z, t)
        //
        template<typename Expr>
        struct eval<Expr, proto::tag::function> {

            // sme terminals
            template<typename Ignore0, std::size_t IsSmeTerminal>
            struct helper {
                template<typename, std::size_t SmeTerminalId>
                struct inner_helper;

                // original function
                template<typename Ignore1>
                struct inner_helper<Ignore1, terminal::id::original_function> {
                    template<std::size_t Begin, std::size_t End>
                    struct collect_and_call {
                        template<typename VarStore, typename Tuple>
                        auto operator()(Expr const& expr, VarStore const& varstore, Tuple const& t) const {
                            auto arg = evaluate(proto::child_c<Begin>(expr), varstore);
                            decltype(auto) value = eval_result::get_value(arg);
                            auto new_varstore = varstore::create_view(eval_result::get_varstore(arg), varstore);

                            return collect_and_call<Begin+1, End>{}(expr, new_varstore, std::tuple_cat(t, std::tie(value)));
                        }
                    };


                    template<std::size_t End>
                    struct collect_and_call<End, End> {
                        template<typename VarStore, typename Tuple>
                        auto operator()(Expr const& expr, VarStore const& varstore, Tuple const& t) const {
                            // expand the collected values from tuple to function
                            return tuple_utility::expand(t, proto::value(proto::child_c<0>(expr)));
                        }
                    };


                    template<typename VarStore>
                    auto operator()(Expr const& expr, VarStore const& varstore) const {
                        auto value = collect_and_call<1, proto::arity_of<Expr const>::value>{}(expr, varstore, std::tuple<>{});

                        // return computed value
                        return eval_result::create(value, varstore::empty());
                    }
                };


                // scoped
                template<typename Ignore1>
                struct inner_helper<Ignore1, terminal::id::scoped> {
                    template<typename VarStore>
                    auto operator()(Expr const& expr, VarStore const& varstore) const {
                        auto result = evaluate(proto::child_c<1>(expr), varstore);
                        return eval_result::create(eval_result::get_value(result), varstore::empty());
                    }
                };


                template<typename VarStore>
                auto operator()(Expr const& expr, VarStore const& varstore) const {
                    return inner_helper<
                        void,
                        terminal::sme_terminal_expr_id<
                            typename proto::result_of::child_c<Expr const, 0>::type
                        >::value
                    >{}(expr, varstore);
                }
            };


            // expr function
            template<typename Ignore0>
            struct helper<Ignore0, 0> {
                template<std::size_t Begin, std::size_t End>
                struct collect_and_call {
                    template<typename VarStore>
                    auto operator()(Expr const& expr, VarStore const& varstore) const {
                        auto arg = evaluate(proto::child_c<Begin>(expr), varstore);
                        auto new_varstore = varstore::create_view(eval_result::get_varstore(arg), varstore);

                        return collect_and_call<Begin+1, End>{}(expr, new_varstore);
                    }
                };

                template<std::size_t End>
                struct collect_and_call<End, End> {
                    template<typename VarStore>
                    auto operator()(Expr const& expr, VarStore const& varstore) const {
                        // looped through all arguments
                        // now evaluate function
                        auto result = evaluate(proto::child_c<0>(expr), varstore);

                        // only return the value, ignore the varstore
                        return eval_result::create(eval_result::get_value(result), varstore::empty());
                    }
                };


                template<typename VarStore>
                auto operator()(Expr const& expr, VarStore const& varstore) const {
                    return collect_and_call<1, proto::arity_of<Expr const>::value>{}(expr, varstore);
                }
            };


            template<typename VarStore>
            auto operator()(Expr const& expr, VarStore const& varstore) const {
                return helper<
                    void,
                    terminal::is_sme_terminal_expr<
                        typename proto::result_of::child_c<Expr const, 0>::type
                    >::value
                >{}(expr, varstore);
            }

        };


        //
        // (x + y) ->* z
        // compute (x + y), return z
        //
        template<typename Expr>
        struct eval<Expr, proto::tag::mem_ptr> {
            template<typename VarStore>
            auto operator()(Expr const& expr, VarStore const& varstore) const {
                auto computed_lhs = evaluate(proto::left(expr), varstore);
                auto computed_rhs = evaluate(
                    proto::right(expr),
                    varstore::create_view(eval_result::get_varstore(computed_lhs), varstore)
                );

                auto new_varstore = varstore::create_view(
                    eval_result::get_varstore(computed_rhs),
                    eval_result::get_varstore(computed_lhs)
                );

                return eval_result::create(
                    eval_result::get_value(computed_rhs),
                    varstore::varstore_from_view(new_varstore)
                );
            }
        };


        //
        // x[y]
        //
        template<typename Expr>
        struct eval<Expr, proto::tag::subscript> {
            template<typename VarStore>
            auto operator()(Expr const& expr, VarStore const& varstore) const {
                auto computed_lhs = evaluate(proto::left(expr), varstore);
                auto computed_rhs = evaluate(
                    proto::right(expr),
                    varstore::create_view(eval_result::get_varstore(computed_lhs), varstore)
                );

                auto new_varstore = varstore::create_view(
                    eval_result::get_varstore(computed_rhs),
                    eval_result::get_varstore(computed_lhs)
                );

                return eval_result::create(
                    eval_result::get_value(computed_lhs)[eval_result::get_value(computed_rhs)],
                    varstore::varstore_from_view(new_varstore)
                );
            }
        };


        //
        // x, y, z
        //
        template<typename Expr>
        struct eval<Expr, proto::tag::comma> {
            template<typename VarStore>
            auto operator()(Expr const& expr, VarStore const& varstore) const {
                auto computed_lhs = evaluate(proto::left(expr), varstore);
                auto computed_rhs = evaluate(
                    proto::right(expr),
                    varstore::create_view(eval_result::get_varstore(computed_lhs), varstore)
                );

                auto new_varstore = varstore::create_view(
                    eval_result::get_varstore(computed_rhs),
                    eval_result::get_varstore(computed_lhs)
                );

                return eval_result::create(
                    eval_result::get_value(computed_rhs),
                    varstore::varstore_from_view(new_varstore)
                );
            }
        };


}} // namespace sme::detail


#endif // EVALUATE_HPP
