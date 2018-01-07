#ifndef SME_HPP
#define SME_HPP


#include <cstddef>
#include <type_traits>
#include <boost/proto/proto.hpp>

#include "detail/evaluate.hpp"
#include "detail/domain.hpp"
#include "detail/terminal.hpp"
#include "detail/varstore.hpp"


namespace sme {


    namespace {
        sme::detail::domain::expr<
            boost::proto::terminal<
                sme::detail::terminal::scoped<>
            >::type
        > const scoped = {};
    }


    template<typename Expr>
    auto evaluate(Expr const& expr) {
        return detail::eval_result::get_value(detail::evaluate(expr, detail::varstore::empty()));
    }


} // namespace sme


// each var must have unique id
#define SME_DEFINE_VAR(NAME, ID)                                                                                            \
    sme::detail::domain::expr<                                                                                              \
        boost::proto::terminal<                                                                                             \
            sme::detail::terminal::var<                                                                                     \
                std::integral_constant<std::size_t, ID>                                                                     \
            >                                                                                                               \
        >::type                                                                                                             \
    > const NAME = {};


#define SME_DEFINE_FUNCTION(NAME, FUNCTION)                                                                                 \
    struct NAME {                                                                                                           \
        template<typename... Args>                                                                                          \
        decltype(auto) operator()(Args&&... args) const {                                                                   \
            return FUNCTION(std::forward<Args>(args)...);                                                                   \
        }                                                                                                                   \
    };                                                                                                                      \
                                                                                                                            \
    sme::detail::domain::expr<                                                                                              \
        typename boost::proto::terminal<                                                                                    \
            sme::detail::terminal::original_function<NAME>                                                                  \
        >::type                                                                                                             \
    > const NAME = {};


#endif // SME_HPP
