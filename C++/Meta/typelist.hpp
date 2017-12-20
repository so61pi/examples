#ifndef TYPELIST_HPP
#define TYPELIST_HPP


#include <cstddef>
#include <type_traits>


namespace tl {


//
// typelist definition
//
template<typename... Ts>
struct typelist;

template<typename T, typename... Ts>
struct typelist<T, Ts...> {
    using first_type = T;
    using next = typelist<Ts...>;

    using size = std::integral_constant<std::size_t, next::size::value + 1>;
};

template<>
struct typelist<> {
    using size = std::integral_constant<std::size_t, 0>;
};


//
//
//
template<typename TypeList>
struct is_typelist {
    using type = std::false_type;
};

template<typename... Ts>
struct is_typelist<typelist<Ts...>> {
    using type = std::true_type;
};

template<typename TypeList>
using is_typelist_t = typename is_typelist<TypeList>::type;

#define TYPELIST_ASSERT(TypeList) \
    static_assert(is_typelist_t<TypeList>::value, "not a typelist");


//
// get size of typelist
//
template<typename TypeList>
struct size {
    TYPELIST_ASSERT(TypeList)
    
    using type = typename TypeList::size;
};

template<typename TypeList>
using size_t = typename size<TypeList>::type;


//
// check if the typelist is empty or not
//
template<typename TypeList>
struct is_empty {
    TYPELIST_ASSERT(TypeList)
    
    using type = std::false_type;
};

template<>
struct is_empty<typelist<>> {
    using type = std::true_type;
};

template<typename TypeList>
using is_empty_t = typename is_empty<TypeList>::type;


//
// get the type at index
//
template<typename TypeList, typename IntegralIndex>
struct at {
    TYPELIST_ASSERT(TypeList)
    
    static_assert(IntegralIndex::value < size<TypeList>::type::value, "index out of range");

    using type = typename at<typename TypeList::next, std::integral_constant<std::size_t, IntegralIndex::value - 1>>::type;
};

template<typename TypeList>
struct at<TypeList, std::integral_constant<std::size_t, 0>> {
    TYPELIST_ASSERT(TypeList)
    
    using type = typename TypeList::first_type;
};

template<typename TypeList, typename IntegralIndex>
using at_t = typename at<TypeList, IntegralIndex>::type;

// for non-type parameter
template<typename TypeList, std::size_t Index>
using at_c = at<TypeList, std::integral_constant<std::size_t, Index>>;

template<typename TypeList, std::size_t Index>
using at_c_t = typename at_c<TypeList, Index>::type;


//
// get the first type
//
template<typename TypeList>
struct front {
    TYPELIST_ASSERT(TypeList)
    static_assert(!is_empty_t<TypeList>::value, "empty typelist");
    
    using type = typename TypeList::first_type;
};

template<typename TypeList>
using front_t = typename front<TypeList>::type;


//
// push a new type to the front
//
template<typename TypeList, typename NewType>
struct push_front;

template<typename... Ts, typename NewType>
struct push_front<typelist<Ts...>, NewType> {
    using type = typelist<NewType, Ts...>;
};

template<typename TypeList, typename NewType>
using push_front_t = typename push_front<TypeList, NewType>::type;


//
// push a new type to the back
//
template<typename TypeList, typename NewType>
struct push_back {
    TYPELIST_ASSERT(TypeList)
    
private:
    using sub = typename push_back<typename TypeList::next, NewType>::type;

public:
    using type = typename push_front<sub, typename TypeList::first_type>::type;
};

template<typename NewType>
struct push_back<typelist<>, NewType> {
    using type = typelist<NewType>;
};

template<typename TypeList, typename NewType>
using push_back_t = typename push_back<TypeList, NewType>::type;


//
// reverse the typelist
//
template<typename TypeList>
struct reverse {
    TYPELIST_ASSERT(TypeList)
    
private:
    using reversed_sub = typename reverse<typename TypeList::next>::type;

public:
    using type = typename push_back<reversed_sub, typename TypeList::first_type>::type;
};

template<>
struct reverse<typelist<>> {
    using type = typelist<>;
};

template<typename TypeList>
using reverse_t = typename reverse<TypeList>::type;


//
// pop type at the front out
//
template<typename TypeList>
struct pop_front {
    TYPELIST_ASSERT(TypeList)
    
    using type = typename TypeList::next;
};

template<typename TypeList>
using pop_front_t = typename pop_front<TypeList>::type;


//
// pop type at the back out
//
template<typename TypeList>
struct pop_back {
    TYPELIST_ASSERT(TypeList)
    static_assert(!is_empty_t<TypeList>::value, "empty typelist");
    
private:
    using temp = typename pop_front<typename reverse<TypeList>::type>::type;

public:
    using type = typename reverse<temp>::type;
};

template<typename TypeList>
using pop_back_t = typename pop_back<TypeList>::type;


//
//
//
template<typename TypeList, typename NewType, typename IntegralIndex>
struct insert {
    TYPELIST_ASSERT(TypeList)
    static_assert(IntegralIndex::value <= size<TypeList>::type::value, "index out of range");
    
private:
    using sub = typename insert<typename pop_front<TypeList>::type, NewType, std::integral_constant<std::size_t, IntegralIndex::value - 1>>::type;

public:
    using type = typename push_front<sub, typename front<TypeList>::type>::type;
};

template<typename TypeList, typename NewType>
struct insert<TypeList, NewType, std::integral_constant<std::size_t, 0>> {
    TYPELIST_ASSERT(TypeList)
    
    using type = typename push_front<TypeList, NewType>::type;
};

template<typename TypeList, typename NewType, typename IntegralIndex>
using insert_t = typename insert<TypeList, NewType, IntegralIndex>::type;

// for non-type parameter
template<typename TypeList, typename NewType, std::size_t Index>
using insert_c = insert<TypeList, NewType, std::integral_constant<std::size_t, Index>>;

template<typename TypeList, typename NewType, std::size_t Index>
using insert_c_t = typename insert_c<TypeList, NewType, Index>::type;


//
//
//
template<typename TypeList, typename IntegralIndex>
struct remove {
    TYPELIST_ASSERT(TypeList)
    static_assert(IntegralIndex::value < size<TypeList>::type::value, "index out of range");
    
private:
    using sub = typename remove<typename pop_front<TypeList>::type, std::integral_constant<std::size_t, IntegralIndex::value - 1>>::type;

public:
    using type = typename push_front<sub, typename front<TypeList>::type>::type;
};

template<typename TypeList>
struct remove<TypeList, std::integral_constant<std::size_t, 0>> {
    TYPELIST_ASSERT(TypeList)
    
    using type = typename pop_front<TypeList>::type;
};

template<typename TypeList, typename IntegralIndex>
using remove_t = typename remove<TypeList, IntegralIndex>::type;

// for non-type parameter
template<typename TypeList, std::size_t Index>
using remove_c = remove<TypeList, std::integral_constant<std::size_t, Index>>;

template<typename TypeList, std::size_t Index>
using remove_c_t = typename remove_c<TypeList, Index>::type;


//
// concatenate 2 typelists
//
template<typename Left, typename Right>
struct concat {
    TYPELIST_ASSERT(Left)
    TYPELIST_ASSERT(Right)
    
private:
    using newLeft = typename push_back<Left, typename Right::first_type>::type;
    using newRight = typename Right::next;

public:
    using type = typename concat<newLeft, newRight>::type;
};

template<typename Left>
struct concat<Left, typelist<>> {
    TYPELIST_ASSERT(Left)
    
    using type = Left;
};

template<typename Left, typename Right>
using concat_t = typename concat<Left, Right>::type;


} // namespace tl


#endif // TYPELIST_HPP
