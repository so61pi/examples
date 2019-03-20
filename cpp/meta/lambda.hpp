#ifndef LAMBDA_HPP
#define LAMBDA_HPP


#include "typelist.hpp"


using namespace tl;


#define MAKE_PLACEHOLDER(index) \
    using _##index = std::integral_constant<std::size_t, index>;

MAKE_PLACEHOLDER(0)
MAKE_PLACEHOLDER(1)
MAKE_PLACEHOLDER(2)
MAKE_PLACEHOLDER(3)
MAKE_PLACEHOLDER(4)
MAKE_PLACEHOLDER(5)
MAKE_PLACEHOLDER(6)
MAKE_PLACEHOLDER(7)
MAKE_PLACEHOLDER(8)
MAKE_PLACEHOLDER(9)

#undef MAKE_PLACEHOLDER


// lambda a metafunction to metafunction class
template<typename Expression>
struct lambda;


template<typename T>
struct lambda {
    using type =
        struct {
            template<typename...>
            struct apply {
                using type = T;
            };
        };
};


template<std::size_t Index>
struct lambda<std::integral_constant<std::size_t, Index>> {
    using type =
        struct {
            template<typename... Xs>
            struct apply {
                static_assert(Index < sizeof...(Xs), "index out of range");
                
                using type = at_t<typelist<Xs...>, std::integral_constant<std::size_t, Index>>;
            };
        };
};


template<template<typename...> class MetaFunction, typename... Ts>
struct lambda<MetaFunction<Ts...>> {
    using type =
        struct {
        private:
            // create a typelist of sub metafunction class
            using sub_metaclass_list = typelist<typename lambda<Ts>::type...>;

            template<typename SubMetaClassList, typename... Xs>
            struct apply_help;
            
            // apply each SubMetaClass, then pass to MetaFunction
            template<typename... SubMetaClass, typename... Xs>
            struct apply_help<typelist<SubMetaClass...>, Xs...> {
                using type = typename MetaFunction<typename SubMetaClass::template apply<Xs...>::type...>::type;
            };

        public:
            template<typename... Xs>
            struct apply {
                using type = typename apply_help<sub_metaclass_list, Xs...>::type;
            };
        };
};


#endif // LAMBDA_HPP
