#include <cstddef>
#include <iterator>
#include <type_traits>


template<typename Category>
struct get_category_index;

template<std::size_t Index>
struct get_category;

#define GET_ITERATOR_CATEGORY(Cat)                          \
    template<>                                              \
    struct get_category<get_category_index<Cat>::value> {   \
        using type = Cat;                                   \
    };


#define CREATE_ITERATOR_INDEX(Cat, Val)             \
    template<>                                      \
    struct get_category_index<Cat> {                \
        static constexpr std::size_t value = Val;   \
    };                                              \
    GET_ITERATOR_CATEGORY(Cat)


CREATE_ITERATOR_INDEX(std::input_iterator_tag, 0)
CREATE_ITERATOR_INDEX(std::forward_iterator_tag, 1)
CREATE_ITERATOR_INDEX(std::bidirectional_iterator_tag, 2)
CREATE_ITERATOR_INDEX(std::random_access_iterator_tag, 3)

#undef CREATE_ITERATOR_INDEX
#undef GET_ITERATOR_CATEGORY


template<typename... Categories>
struct iterator_minimum_category;

template<typename First, typename Second, typename... Rest>
struct iterator_minimum_category<First, Second, Rest...> {
    template<std::size_t A, std::size_t B>
    struct min {
        static constexpr std::size_t value = (A < B) ? A : B;
    };

    using type = typename iterator_minimum_category<
        typename get_category<
            min<
                get_category_index<First>::value, 
                get_category_index<Second>::value
            >::value
        >::type,
        Rest...
    >::type;
};

template<typename First>
struct iterator_minimum_category<First> {
    using type = First;
};


int main() {
    static_assert(
        std::is_same<
            iterator_minimum_category<std::random_access_iterator_tag, std::forward_iterator_tag>::type,
            std::forward_iterator_tag
        >::value,
        "error"
    );
}
