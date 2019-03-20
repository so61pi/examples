#ifndef VAR_STORE_HPP
#define VAR_STORE_HPP


#include <cstddef>
#include <tuple>
#include <type_traits>
#include <boost/fusion/include/as_list.hpp>
#include <boost/fusion/include/deref.hpp>
#include <boost/fusion/include/empty.hpp>
#include <boost/fusion/include/end.hpp>
#include <boost/fusion/include/find_if.hpp>
#include <boost/fusion/include/join.hpp>
#include <boost/fusion/include/make_list.hpp>
#include <boost/mpl/bool.hpp>

#include "terminal.hpp"


namespace sme { namespace detail {


    namespace varstore {


        namespace fusion = boost::fusion;
        namespace mpl = boost::mpl;


        // used by fusion::find_if to find a var
        template<typename Tuple, typename VarId>
        struct find_var {
            using Tuple_ = std::remove_cv_t<std::remove_reference_t<Tuple>>;

            using type =
                typename std::conditional<
                    std::is_same<
                        typename std::tuple_element<0, Tuple_>::type,
                        VarId
                    >::value,
                    mpl::true_,
                    mpl::false_
                >::type;
        };


        // check if a var exists or not
        template<typename VarStore, typename VarId>
        struct check_var_existence {
            static_assert(
                terminal::is_var<VarId>::value,
                "\n\n\n\n*****     invalid var type     *****\n\n\n"
            );

            static std::size_t const value =
                !fusion::result_of::empty<VarStore const>::type::value &&
                !std::is_same<
                    typename fusion::result_of::find_if<
                        VarStore const,
                        find_var<mpl::_, VarId>
                    >::type,
                    typename fusion::result_of::end<VarStore const>::type
                >::value;
        };


        // get a var from varstore
        template<typename VarId, typename VarStore>
        decltype(auto) get_var(VarStore const& store) {
            static_assert(
                check_var_existence<VarStore const, VarId>::value,
                "\n\n\n\n*****     cannot find var     *****\n\n\n"
            );

            return fusion::deref(fusion::find_if<find_var<mpl::_, VarId>>(store));
        }


        // create varstore view from a list of varstores
        template<typename VS1, typename VS2>
        decltype(auto) create_view(VS1 const& s1, VS2 const& s2) {
            return fusion::join(s1, s2);
        }

        template<typename VSFirst, typename... VSRest>
        decltype(auto) create_view(VSFirst const& sfirst, VSRest const&... srest) {
            return create_view(sfirst, create_view(srest...));
        }


        // turn varstore view to real varstore
        template<typename VarStoreView>
        decltype(auto) varstore_from_view(VarStoreView const& view) {
            return fusion::as_list(view);
        }


        // create varstore from a list of vars
        template<typename... Var>
        decltype(auto) varstore_from_var(Var const&... var) {
            return fusion::make_list(var...);
        }


        // create an empty varstore
        auto empty() {
            return fusion::nil{};
        }


    } // namespace varstore


}} // namespace sme::detail


#endif // VAR_STORE_HPP
