#ifndef VAR_HPP
#define VAR_HPP


#include <tuple>
#include <utility>


namespace sme { namespace detail {


    namespace var {


        // create a var
        template<typename VarId, typename Value>
        auto create_var(Value&& value) {
            return std::make_tuple(VarId{}, std::forward<Value>(value));
        }


        //
        template<typename Var>
        auto get_value(Var const& var) {
            return std::get<1>(var);
        }


    } // namespace var


}} // namespace sme::detail


#endif // VAR_HPP
