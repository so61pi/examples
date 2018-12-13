#include <cassert>
#include <cstddef>
#include <forward_list>
#include <functional>
#include <iostream>
#include <iterator>
#include <list>
#include <string>
#include <tuple>
#include <type_traits>
#include <utility>
#include <vector>


//
//
//
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


//
//
//
template<typename Iterator>
class single_container_picker_iterator {
    using this_type = single_container_picker_iterator;
    using iterator_type = Iterator;


    struct iterator_index {
        enum : std::size_t {
            current,
            end
        };
    };


public:
    single_container_picker_iterator() = default;

    explicit single_container_picker_iterator(std::vector<std::tuple<iterator_type, iterator_type>> its_pairs) :
        m_its_pairs{ std::move(its_pairs) }
    {
        construct_end_iterators();
    }

    template<typename... IteratorPairs>
    explicit single_container_picker_iterator(IteratorPairs... its_pairs) :
        m_its_pairs{ std::move(its_pairs)... }
    {
        construct_end_iterators();
    }


    auto operator++() -> this_type& {
        assert(m_its_pairs.size() != 0 && "cannot increase empty iterator");
        next_impl(m_its_pairs.size() - 1);
        return *this;
    }

    auto operator++(int) {
        auto retval = *this;
        ++(*this);
        return retval;
    }


    auto operator--() -> this_type& {
        assert(m_its_pairs.size() != 0 && "cannot decrease empty iterator");
        prev_impl(m_its_pairs.size() - 1);
        return *this;
    }

    auto operator--(int) {
        auto retval = *this;
        --(*this);
        return retval;
    }


    auto operator*() const -> decltype(auto) {
        assert(m_its_pairs.size() != 0 && "cannot dereference empty iterator");
        return values();
    }


    auto operator==(this_type const& rhs) const -> bool {
        auto iters_equal = [](auto const& lhs_iterators, auto const& rhs_iterators) -> bool {
            return std::equal(
                begin(lhs_iterators), end(lhs_iterators),
                begin(rhs_iterators), end(rhs_iterators),
                [](auto const& a, auto const& b) -> bool {
                    return (a.get() == b.get());
                }
            );
        };

        return iters_equal(current_iterators(), rhs.current_iterators())
            && iters_equal(end_iterators(), rhs.end_iterators());
    }


    auto operator!=(this_type const& rhs) const -> bool {
        return !(*this == rhs);
    }


private:
    //
    void next_impl(std::size_t index) {
        ++std::get<iterator_index::current>(m_its_pairs[index]);
        if (index != 0) {
            if (std::get<iterator_index::current>(m_its_pairs[index]) == std::get<iterator_index::end>(m_its_pairs[index])) {
                next_impl(index - 1);
                std::get<iterator_index::current>(m_its_pairs[index]) = std::get<iterator_index::current>(m_its_pairs[index - 1]) + 1;
            }
        }
    }


    //
    void prev_impl(std::size_t index) {
        --std::get<iterator_index::current>(m_its_pairs[index]);
        if (index != 0) {
            if (std::get<iterator_index::current>(m_its_pairs[index]) == std::get<iterator_index::current>(m_its_pairs[index - 1])) {
                prev_impl(index - 1);
                std::get<iterator_index::current>(m_its_pairs[index]) = std::get<iterator_index::end>(m_its_pairs[index]) - 1;
            }
        }
    }


    //
    std::vector<std::reference_wrapper<iterator_type const>> m_end_iterators_cache;
    void construct_end_iterators() {
        m_end_iterators_cache.reserve(m_its_pairs.size());
        for (auto const& its_pair : m_its_pairs) {
            m_end_iterators_cache.emplace_back(std::get<iterator_index::end>(its_pair));
        }
    }

    auto end_iterators() const -> decltype(auto) {
        auto const& retref = m_end_iterators_cache;
        return retref;
    }


    //
    mutable std::vector<std::reference_wrapper<iterator_type const>> m_current_iterators_buffer;
    auto current_iterators() const -> decltype(auto) {
        m_current_iterators_buffer.clear();
        m_current_iterators_buffer.reserve(m_its_pairs.size());
        for (auto const& its_pair : m_its_pairs) {
            m_current_iterators_buffer.emplace_back(std::get<iterator_index::current>(its_pair));
        }
        auto const& retref = m_current_iterators_buffer;
        return retref;
    }


    //
    mutable std::vector<std::reference_wrapper<typename std::iterator_traits<iterator_type>::value_type>> m_value_refs_buffer;
    auto values() const -> decltype(auto) {
        m_value_refs_buffer.clear();
        m_value_refs_buffer.reserve(m_its_pairs.size());
        for (auto& its_pair : m_its_pairs) {
            m_value_refs_buffer.emplace_back(*std::get<iterator_index::current>(its_pair));
        }
        auto const& retref = m_value_refs_buffer;
        return retref;
    }


private:
    // tuple<current, end>
    std::vector<std::tuple<iterator_type, iterator_type>> m_its_pairs;


public: // iterator traits
    using value_type = decltype(*std::declval<this_type>());
    using pointer = void;
    using reference = value_type;
    using difference_type = std::ptrdiff_t;
    using iterator_category = typename iterator_minimum_category<
        std::bidirectional_iterator_tag,
        typename std::iterator_traits<iterator_type>::iterator_category
    >::type;
};


template<typename Iterator>
class single_container_picker {
    using iterator_type = Iterator;


    struct iterator_index {
        enum : std::size_t {
            current,
            end
        };
    };


public:
    single_container_picker(iterator_type first, iterator_type last, std::size_t count) {
        std::vector<std::tuple<iterator_type, iterator_type>> its_pairs;
        its_pairs.reserve(count);
        auto dis = std::distance(first, last);
        auto delta = dis - count + 1;

        for (auto i = 0U; i < count; ++i) {
            its_pairs.emplace_back(first + i, first + i + delta);
        }
        
        // constructs begin iterators
        m_begin_iterators = single_container_picker_iterator<iterator_type>{ its_pairs };
        
        // constructs end iterators
        std::vector<std::tuple<iterator_type, iterator_type>> v;
        v.reserve(its_pairs.size());
        for (auto& its_pair : its_pairs) {
            v.emplace_back(std::get<iterator_index::end>(its_pair), std::get<iterator_index::end>(its_pair));
        }
        m_end_iterators = single_container_picker_iterator<iterator_type>{ v };
    }


    auto begin() const -> decltype(auto) {
        auto const& retref = m_begin_iterators;
        return retref;
    }


    auto end() const -> decltype(auto) {
        auto const& retref = m_end_iterators;
        return retref;
    }


private:
    single_container_picker_iterator<iterator_type> m_begin_iterators;
    single_container_picker_iterator<iterator_type> m_end_iterators;
    
};


template<typename Iterator>
auto begin(single_container_picker<Iterator> const& scp) -> decltype(auto) {
    return scp.begin();
}


template<typename Iterator>
auto end(single_container_picker<Iterator> const& scp) -> decltype(auto) {
    return scp.end();
}


template<typename Iterator>
auto make_single_container_picker(Iterator first, Iterator last, std::size_t count) {
    assert(static_cast<std::size_t>(std::distance(first, last)) >= count);
    return single_container_picker<Iterator>{ first, last, count };
}


//
//
//
template<typename Iterator>
class multiple_containers_picker_iterator {
    using this_type = multiple_containers_picker_iterator;
    using iterator_type = Iterator;


    struct iterator_index {
        enum : std::size_t {
            begin,
            current,
            end
        };
    };


public:
    multiple_containers_picker_iterator() = default;

    explicit multiple_containers_picker_iterator(std::vector<std::tuple<iterator_type, iterator_type, iterator_type>> its_triples) :
        m_its_triples{ std::move(its_triples) }
    {
        construct_begin_iterator_refs();
        construct_end_iterator_refs();
    }

    template<typename... IteratorTriples>
    explicit multiple_containers_picker_iterator(IteratorTriples... its_triples) :
        m_its_triples{ std::move(its_triples)... }
    {
        construct_begin_iterator_refs();
        construct_end_iterator_refs();
    }


    auto operator++() -> this_type& {
        assert(m_its_triples.size() != 0 && "cannot increase empty iterator");
        next_impl(m_its_triples.size() - 1);
        return *this;
    }

    auto operator++(int) {
        auto retval = *this;
        ++(*this);
        return retval;
    }


    auto operator--() -> this_type& {
        assert(m_its_triples.size() != 0 && "cannot decrease empty iterator");
        if (current_iterators() == end_iterators()) {
            prev_impl_special();
        }
        else {
            prev_impl(m_its_triples.size() - 1);
        }
        return *this;
    }

    auto operator--(int) {
        auto retval = *this;
        --(*this);
        return retval;
    }


    auto operator*() const ->decltype(auto) {
        assert(m_its_triples.size() != 0 && "cannot dereference empty iterator");
        return values();
    }


    auto operator==(this_type const& rhs) const -> bool {
        auto iters_equal = [](auto const& lhs_iterators, auto const& rhs_iterators) -> bool {
            return std::equal(
                begin(lhs_iterators), end(lhs_iterators),
                begin(rhs_iterators), end(rhs_iterators),
                [](auto const& a, auto const& b) -> bool {
                    return (a.get() == b.get());
                }
            );
        };
        
        return iters_equal(begin_iterators(), rhs.begin_iterators())
            && iters_equal(current_iterators(), rhs.current_iterators())
            && iters_equal(end_iterators(), rhs.end_iterators());
    }


    auto operator!=(this_type const& rhs) const -> bool {
        return !(*this == rhs);
    }


private:
    //
    auto next_impl(std::size_t index) -> bool {
        bool reset = true;
        ++std::get<iterator_index::current>(m_its_triples[index]);
        if (std::get<iterator_index::current>(m_its_triples[index]) == std::get<iterator_index::end>(m_its_triples[index])) {
            if (index == 0) {
                reset = false;
            }
            else {
                reset = next_impl(index - 1);
                if (reset) {
                    std::get<iterator_index::current>(m_its_triples[index]) = std::get<iterator_index::begin>(m_its_triples[index]);
                }
            }
        }
        return reset;
    }


    //
    auto prev_impl(std::size_t index) -> bool {
        auto prev_helper = [](auto& its_triple) {
            if (std::get<iterator_index::current>(its_triple) == std::get<iterator_index::begin>(its_triple))
                std::get<iterator_index::current>(its_triple) = std::get<iterator_index::end>(its_triple);
            else
                --std::get<iterator_index::current>(its_triple);
        };

        bool reset = true;
        prev_helper(std::get<iterator_index::current>(m_its_triples[index]));
        if (std::get<iterator_index::current>(m_its_triples[index]) == std::get<iterator_index::end>(m_its_triples[index])) {
            if (index == 0) {
                reset = false;
            }
            else {
                reset = prev_impl(index - 1);
                if (reset) {
                    std::advance(std::get<iterator_index::current>(m_its_triples[index]), -1);
                }
            }
        }
        return reset;
    }

    void prev_impl_special() {
        for (auto& its_triple : m_its_triples) {
            std::get<iterator_index::current>(its_triple) = std::prev(std::get<iterator_index::end>(its_triple));
        }
    }


    //
    template<std::size_t IteratorIndex, typename Buffer>
    void iterators(Buffer& buffer) const {
        for (auto& its_triple : m_its_triples) {
            buffer.emplace_back(std::get<IteratorIndex>(its_triple));
        }
    }


    //
    std::vector<std::reference_wrapper<iterator_type const>> m_begin_iterator_refs_cache;
    void construct_begin_iterator_refs() {
        m_begin_iterator_refs_cache.reserve(m_its_triples.size());
        iterators<iterator_index::begin>(m_begin_iterator_refs_cache);
    }

    auto begin_iterators() const -> decltype(auto) {
        auto const& retref = m_begin_iterator_refs_cache;
        return retref;
    }


    //
    std::vector<std::reference_wrapper<iterator_type const>> m_end_iterator_refs_cache;
    void construct_end_iterator_refs() {
        m_end_iterator_refs_cache.reserve(m_its_triples.size());
        iterators<iterator_index::end>(m_end_iterator_refs_cache);
    }

    auto end_iterators() const -> decltype(auto) {
        auto const& retref = m_end_iterator_refs_cache;
        return retref;
    }


    //
    mutable std::vector<std::reference_wrapper<iterator_type const>> m_current_iterator_refs_buffer;
    auto current_iterators() const -> decltype(auto) {
        m_current_iterator_refs_buffer.clear();
        m_current_iterator_refs_buffer.reserve(m_its_triples.size());
        iterators<iterator_index::current>(m_current_iterator_refs_buffer);
        
        auto const& retref = m_current_iterator_refs_buffer;
        return retref;
    }


    //
    mutable std::vector<std::reference_wrapper<typename std::iterator_traits<iterator_type>::value_type>> m_value_refs_buffer;
    auto values() const -> decltype(auto) {
        m_value_refs_buffer.clear();
        m_value_refs_buffer.reserve(m_its_triples.size());
        for (auto& its_triple : m_its_triples) {
            m_value_refs_buffer.emplace_back(*std::get<iterator_index::current>(its_triple));
        }
        
        auto const& retref = m_value_refs_buffer;
        return retref;
    }


private:
    // tuple<begin, current, end>
    std::vector<std::tuple<iterator_type, iterator_type, iterator_type>> m_its_triples;


public: // iterator traits
    using value_type = decltype(*std::declval<this_type>());
    using pointer = void;
    using reference = value_type;
    using difference_type = std::ptrdiff_t;
    using iterator_category = typename iterator_minimum_category<
        std::bidirectional_iterator_tag,
        typename std::iterator_traits<iterator_type>::iterator_category
    >::type;
};


template<typename Iterator>
class multiple_containers_picker {
    using iterator_type = Iterator;


    struct iterator_index {
        enum : std::size_t {
            begin,
            end
        };
    };


public:
    template<typename... IteratorPairs>
    multiple_containers_picker(IteratorPairs... its_pairs) :
        m_begin_iterators{
            std::make_tuple(
                std::get<iterator_index::begin>(its_pairs),
                std::get<iterator_index::begin>(its_pairs),
                std::get<iterator_index::end>(its_pairs)
            )...
        },
        m_end_iterators{
            std::make_tuple(
                std::get<iterator_index::begin>(its_pairs),
                std::get<iterator_index::end>(its_pairs),
                std::get<iterator_index::end>(its_pairs)
            )...
        }
    {}


    auto begin() const noexcept -> decltype(auto) {
        auto const& retref = m_begin_iterators;
        return retref;
    }


    auto end() const noexcept -> decltype(auto) {
        auto const& retref = m_end_iterators;
        return retref;
    }


private:
    multiple_containers_picker_iterator<iterator_type> m_begin_iterators;
    multiple_containers_picker_iterator<iterator_type> m_end_iterators;
};


template<typename Iterator>
auto begin(multiple_containers_picker<Iterator> const& mcp) -> decltype(auto) {
    return mcp.begin();
}


template<typename Iterator>
auto end(multiple_containers_picker<Iterator> const& mcp) -> decltype(auto) {
    return mcp.end();
}


// IteratorPairs = std::tuple<begin, end>...
//                 std::pair<begin, end>...
template<typename... IteratorPairs>
auto make_multiple_containers_picker(IteratorPairs... its_pairs) {
    using iterator_type = std::tuple_element_t<0, std::tuple_element_t<0, std::tuple<IteratorPairs...>>>;

    return multiple_containers_picker<iterator_type>{ its_pairs... };
}


//
//
// IteratorTriples = tuple<begin, current, end>...
template<typename... IteratorTriples>
class different_containers_picker_iterator {
    static_assert(sizeof...(IteratorTriples) != 0, "cannot have empty iterators set");

    static constexpr std::size_t size = sizeof...(IteratorTriples);
    using this_type = different_containers_picker_iterator;


    struct iterator_index {
        enum : std::size_t {
            begin,
            current,
            end
        };
    };


public:
    different_containers_picker_iterator() = default;
    explicit different_containers_picker_iterator(IteratorTriples... its_triples) :
        m_its_triples{ std::move(its_triples)... }
    {}


    auto operator++() -> this_type& {
        next_impl<size - 1>{}(*this);
        return *this;
    }

    auto operator++(int) {
        auto retval = *this;
        ++(*this);
        return retval;
    }


    auto operator--() -> this_type& {
        if (current_iterators() == end_iterators()) {
            prev_impl_special<size - 1>{}(*this);
        }
        else {
            prev_impl<size - 1>{}(*this);
        }
        return *this;
    }

    auto operator--(int) {
        auto retval = *this;
        --(*this);
        return retval;
    }


    auto operator*() -> decltype(auto) {
        return values();
    }


    auto operator==(this_type const& rhs) const {
        return (begin_iterators() == rhs.begin_iterators())
            && (current_iterators() == rhs.current_iterators())
            && (end_iterators() == rhs.end_iterators());
    }


    auto operator!=(this_type const& rhs) const {
        return !(*this == rhs);
    }

    
private:
    //
    template<std::size_t WrapperTupleIndex, typename Ignore = void>
    struct next_impl {
        bool operator()(this_type& outer) const {
            auto& its_triple = std::get<WrapperTupleIndex>(outer.m_its_triples);
            bool reset = true;
            ++std::get<iterator_index::current>(its_triple);
            if (std::get<iterator_index::current>(its_triple) == std::get<iterator_index::end>(its_triple)) {
                reset = next_impl<WrapperTupleIndex - 1, Ignore>{}(outer);
                if (reset) {
                    std::get<iterator_index::current>(its_triple) = std::get<iterator_index::begin>(its_triple);
                }
            }
            return reset;
        }
    };

    template<typename Ignore>
    struct next_impl<0, Ignore> {
        bool operator()(this_type& outer) const {
            auto& its_triple = std::get<0>(outer.m_its_triples);
            bool reset = true;
            ++std::get<iterator_index::current>(its_triple);
            if (std::get<iterator_index::current>(its_triple) == std::get<iterator_index::end>(its_triple)) {
                reset = false;
            }
            return reset;
        }
    };


    //
    struct prev_helper {
        template<typename T>
        void operator()(T& its_triple) {
            if (std::get<iterator_index::current>(its_triple) == std::get<iterator_index::begin>(its_triple))
                std::get<iterator_index::current>(its_triple) = std::get<iterator_index::end>(its_triple);
            else
                --std::get<iterator_index::current>(its_triple);
        }
    };


    //
    template<std::size_t WrapperTupleIndex, typename Ignore = void>
    struct prev_impl {
        bool operator()(this_type& outer) const {
            auto& its_triple = std::get<WrapperTupleIndex>(outer.m_its_triples);
            bool reset = true;
            prev_helper{}(its_triple);
            if (std::get<iterator_index::current>(its_triple) == std::get<iterator_index::end>(its_triple)) {
                reset = prev_impl<WrapperTupleIndex - 1, Ignore>{}(outer);
                if (reset) {
                    std::advance(std::get<iterator_index::current>(its_triple), -1);
                }
            }
            return reset;
        }
    };

    template<typename Ignore>
    struct prev_impl<0, Ignore> {
        bool operator()(this_type& outer) const {
            auto& its_triple = std::get<0>(outer.m_its_triples);
            bool reset = true;
            prev_helper{}(its_triple);
            if (std::get<iterator_index::current>(its_triple) == std::get<iterator_index::end>(its_triple)) {
                reset = false;
            }
            return reset;
        }
    };


    //
    template<std::size_t WrapperTupleIndex, typename Ignore = void>
    struct prev_impl_special {
        void operator()(this_type& outer) const {
            auto& its_triple = std::get<WrapperTupleIndex>(outer.m_its_triples);
            std::get<iterator_index::current>(its_triple) = std::prev(std::get<iterator_index::end>(its_triple));
            prev_impl_special<WrapperTupleIndex - 1, Ignore>{}(outer);
        }
    };

    template<typename Ignore>
    struct prev_impl_special<0, Ignore> {
        void operator()(this_type& outer) const {
            auto& its_triple = std::get<0>(outer.m_its_triples);
            std::get<iterator_index::current>(its_triple) = std::prev(std::get<iterator_index::end>(its_triple));
        }
    };


    //
    template<std::size_t IteratorIndex, std::size_t... WrapperTupleIndices>
    auto iterators_helper(std::index_sequence<WrapperTupleIndices...>) const {
        return std::make_tuple(
            std::ref(
                std::get<IteratorIndex>(
                    std::get<WrapperTupleIndices>(m_its_triples)
                )
            )...
        );
    }


    //
    template<std::size_t IteratorIndex>
    auto iterators() const {
        return iterators_helper<IteratorIndex>(std::make_index_sequence<size>{});
    }


    //
    auto begin_iterators() const {
        return iterators<iterator_index::begin>();
    }


    //
    auto end_iterators() const {
        return iterators<iterator_index::end>();
    }


    //
    auto current_iterators() const {
        return iterators<iterator_index::current>();
    }


    //
    template<std::size_t... WrapperTupleIndices>
    auto values_helper(std::index_sequence<WrapperTupleIndices...>) const {
        return std::make_tuple(
            std::ref(
                *std::get<iterator_index::current>(
                    std::get<WrapperTupleIndices>(m_its_triples)
                )
            )...
        );
    }


    //
    auto values() const {
        return values_helper(std::make_index_sequence<size>{});
    }


private:
    std::tuple<IteratorTriples...> m_its_triples;


public: // iterator traits
    using value_type = decltype(*std::declval<this_type>());
    using pointer = void;
    using reference = value_type;
    using difference_type = std::ptrdiff_t;
    using iterator_category = typename iterator_minimum_category<
        std::bidirectional_iterator_tag,
        typename std::iterator_traits<
            std::tuple_element_t<iterator_index::begin, IteratorTriples>
        >::iterator_category...
    >::type;
};


// IteratorPairs = tuple<begin, end>...
//                 pair<begin, end>...
template<typename... IteratorPairs>
class different_containers_picker {
    static_assert(sizeof...(IteratorPairs) != 0, "cannot have empty iterators set");
    static constexpr std::size_t size = sizeof...(IteratorPairs);


    struct iterator_index {
        enum : std::size_t {
            begin,
            end
        };
    };


public:
    different_containers_picker(IteratorPairs... its_pairs) :
        m_its_pairs{ std::move(its_pairs)... }
    {}


    auto begin() {
        return begin_helper(std::make_index_sequence<size>{});
    }


    auto end() {
        return end_helper(std::make_index_sequence<size>{});
    }


private:
    template<std::size_t... WrapperTupleIndices>
    auto begin_helper(std::index_sequence<WrapperTupleIndices...>) {
        return different_containers_picker_iterator<
            decltype(
                std::make_tuple(
                    std::get<iterator_index::begin>(std::get<WrapperTupleIndices>(m_its_pairs)),
                    std::get<iterator_index::begin>(std::get<WrapperTupleIndices>(m_its_pairs)),
                    std::get<iterator_index::end>(std::get<WrapperTupleIndices>(m_its_pairs))
                )
            )...
        >{
            std::make_tuple(
                std::get<iterator_index::begin>(std::get<WrapperTupleIndices>(m_its_pairs)),
                std::get<iterator_index::begin>(std::get<WrapperTupleIndices>(m_its_pairs)),
                std::get<iterator_index::end>(std::get<WrapperTupleIndices>(m_its_pairs))
            )...
        };
    }


    template<std::size_t... WrapperTupleIndices>
    auto end_helper(std::index_sequence<WrapperTupleIndices...>) {
        return different_containers_picker_iterator<
            decltype(
                std::make_tuple(
                    std::get<iterator_index::begin>(std::get<WrapperTupleIndices>(m_its_pairs)),
                    std::get<iterator_index::end>(std::get<WrapperTupleIndices>(m_its_pairs)),
                    std::get<iterator_index::end>(std::get<WrapperTupleIndices>(m_its_pairs))
                )
            )...
        >{
            std::make_tuple(
                std::get<iterator_index::begin>(std::get<WrapperTupleIndices>(m_its_pairs)),
                std::get<iterator_index::end>(std::get<WrapperTupleIndices>(m_its_pairs)),
                std::get<iterator_index::end>(std::get<WrapperTupleIndices>(m_its_pairs))
            )...
        };
    }

    
private:
    std::tuple<IteratorPairs...> m_its_pairs;
};


template<typename Iterator>
auto begin(different_containers_picker<Iterator> const& dcp) -> decltype(auto) {
    return dcp.begin();
}


template<typename Iterator>
auto end(different_containers_picker<Iterator> const& dcp) -> decltype(auto) {
    return dcp.end();
}


// IteratorPairs = tuple<begin, end>...
//                 pair<begin, end>...
template<typename... IteratorPairs>
auto make_different_containers_picker(IteratorPairs... its_pairs) {
    return different_containers_picker<IteratorPairs...>{ its_pairs... };
}



//
//
//
template<typename Tuple, std::size_t... Is>
auto print_helper(Tuple&& tuple, std::index_sequence<0, Is...>) -> decltype(auto) {
    return std::make_tuple(std::ref(std::get<Is>(std::forward<Tuple>(tuple)))...);
}

template<typename First>
void print(std::ostream& os, std::tuple<First> const& tuple) {
    os << std::get<0>(tuple);
}

template<typename First, typename... Rest>
void print(std::ostream& os, std::tuple<First, Rest...> const& tuple) {
    os << std::get<0>(tuple) << ", ";
    std::make_index_sequence<1 + sizeof...(Rest)> indices;

    print(os, print_helper(tuple, indices));
}

template<typename... Args>
std::ostream& operator<<(std::ostream& os, std::tuple<Args...> const& t) {
    os << "(";
    print(os, t);
    os << ")";
    return os;
}


int main() {
    std::vector<int> a{ 0,1,2,3,4,5 };
    std::vector<int> b{ 6,7,8 };
    std::list<std::string> c{ "so61pi", "HNT" };
    std::forward_list<double> d{ 2.71, 3.14 };

    auto spicker = make_single_container_picker(begin(a), end(a), 3);
    auto mpicker = make_multiple_containers_picker(
        std::make_tuple(begin(a), end(a)),
        std::make_tuple(begin(b), end(b))
    );
    auto dpicker = make_different_containers_picker(
        std::make_tuple(begin(spicker), end(spicker)),
        std::make_tuple(begin(c), end(c))
    );


    std::cout << "single container picker:\n";
    for (auto&& elements : spicker) {
        for (auto&& e : elements) {
            std::cout << e << ' ';
        }
        std::cout << "\n";
    }
    std::cout << "number of results: " << std::distance(begin(spicker), end(spicker)) << "\n\n";


    std::cout << "multiple containers picker:\n";
    for (auto&& elements : mpicker) {
        for (auto&& e : elements) {
            std::cout << e << ' ';
        }
        std::cout << "\n";
    }
    std::cout << "number of results: " << std::distance(begin(mpicker), end(mpicker)) << "\n\n";


    std::cout << "different containers picker:\n";
    for (auto&& elements : dpicker) {
        for (auto&& e : std::get<0>(elements)) {
            std::cout << e << ' ';
        }
        std::cout << std::get<1>(elements) << "\n";
    }
    std::cout << "number of results: " << std::distance(begin(dpicker), end(dpicker)) << "\n\n";
}
