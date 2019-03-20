#ifndef DOMAIN_HPP
#define DOMAIN_HPP


#include <boost/proto/proto.hpp>


namespace sme { namespace detail {


    namespace domain {


        template<typename Expr>
        struct expr;


        // capture value instead of reference
        struct domain : boost::proto::domain<boost::proto::generator<expr>>
        {
            template<typename T>
            struct as_child : proto_base_domain::as_expr<T>
            {};
        };


        template<typename Expr>
        struct expr : boost::proto::extends<Expr, expr<Expr>, domain>
        {
            using base_type = boost::proto::extends<Expr, expr<Expr>, domain>;

            expr(Expr const& expr = Expr{}) :
                base_type{ expr }
            {}

            BOOST_PROTO_EXTENDS_USING_ASSIGN(expr)
        };


    } // namespace domain


}} // namespace sme::detail


#endif // DOMAIN_HPP
