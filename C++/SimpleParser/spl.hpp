#ifndef SPL_HPP_INCLUDED
#define SPL_HPP_INCLUDED


#include <algorithm>
#include <cctype>
#include <cstddef>
#include <cstring>
#include <functional>
#include <iostream>
#include <iterator>
#include <memory>
#include <sstream>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>


//
// base
//
namespace spl {
    /**
     * @brief The iterator class
     */
    class iterator {
    public:
        using base_iterator     = std::string::const_iterator;
        using iterator_category = base_iterator::iterator_category;
        using value_type        = base_iterator::value_type;
        using difference_type   = base_iterator::difference_type;
        using pointer           = base_iterator::pointer;
        using reference         = base_iterator::reference;


        iterator() = default;
        iterator(base_iterator base) : m_base(base), m_line(1), m_col(1) {}


        auto operator*() const -> reference {
            return m_base.operator*();
        }

        auto operator->() const -> pointer {
            return m_base.operator->();
        }

        auto operator[](difference_type n) const -> reference const& {
            return m_base.operator[](n);
        }

        auto operator[](difference_type n) -> reference& {
            return m_base.operator[](n);
        }

        auto operator++() -> iterator& {
            m_base.operator++();
            if (*m_base == '\n') {
                ++m_line;
                m_col = 1;
            } else {
                ++m_col;
            }
            std::cout << m_line << '-' << m_col << '\n';
            return *this;
        }

        auto operator++(int) -> iterator {
            auto tmp = *this;
            operator++();
            return tmp;
        }

        auto operator--() -> iterator& {
            m_base.operator--();
            return *this;
        }

        auto operator--(int) -> iterator {
            auto tmp = *this;
            operator--();
            return tmp;
        }

        auto operator+=(difference_type n) -> iterator& {
            while (n-- > 0) {
                operator++();
            }
            return *this;
        }

        auto operator-=(difference_type n) -> iterator& {
            m_base.operator-=(n);
            return *this;
        }

        auto base() const -> base_iterator { return m_base; }

        auto line() const -> std::size_t { return m_line; }
        auto column() const -> std::size_t { return m_col; }

    private:
        base_iterator m_base;
        std::size_t   m_line;
        std::size_t   m_col;
    };


    auto operator+(iterator const& it, iterator::difference_type n) -> iterator {
        iterator tmp = it;
        tmp += n;
        return tmp;
    }

    auto operator+(iterator::difference_type n, const iterator& it) -> iterator {
        return it + n;
    }

    auto operator-(iterator const& it, iterator::difference_type n) -> iterator {
        iterator tmp = it;
        tmp -= n;
        return tmp;
    }

    auto operator-(iterator const& lhs, const iterator& rhs) -> iterator::difference_type {
        return lhs.base() - rhs.base();
    }


    bool operator==(const iterator& lhs, const iterator& rhs) {
        return lhs.base() == rhs.base();
    }

    bool operator<(const iterator& lhs, const iterator& rhs) {
        return lhs.base() < rhs.base();
    }

    bool operator!=(const iterator& lhs, const iterator& rhs) {
        return !(lhs == rhs);
    }

    bool operator<=(const iterator& lhs, const iterator& rhs) {
        return lhs == rhs || lhs < rhs;
    }

    bool operator>(const iterator& lhs, const iterator& rhs) {
        return !(lhs <= rhs);
    }

    bool operator>=(const iterator& lhs, const iterator& rhs) {
        return !(lhs < rhs);
    }


    /**
     * @brief The error struct
     */
    struct error {
        int code;
        std::stringstream message;
        iterator where;
    };


    class base_parser {
    public:
        virtual bool parse(iterator& first, iterator const& last,
                           error& error) = 0;
        virtual ~base_parser() = default;

        virtual char const* name() const { return "base-parser"; }
    };
}


namespace spl {
    namespace traits {
        //
        // is_same
        //
        template <typename A, typename B, typename... C>
        struct is_same {
            static bool const value =
                is_same<A, B>::value & is_same<B, C...>::value;
        };

        template <typename A>
        struct is_same<A, A> {
            static bool const value = true;
        };

        template <typename A, typename B>
        struct is_same<A, B> {
            static bool const value = false;
        };


        //
        // is_base_from_base_parser
        //
        template <typename A, typename... B>
        struct is_base_from_base_parser {
            static bool const value = is_base_from_base_parser<A>::value
                                      & is_base_from_base_parser<B...>::value;
        };

        template <typename A>
        struct is_base_from_base_parser<A> {
            static bool const value =
                std::is_base_of<base_parser,
                                typename std::decay<A>::type>::value;
        };


        //
        // enable_if_base_from_base_parser
        //
        template <typename A, typename... B>
        struct enable_if_base_from_base_parser
            : std::enable_if<is_base_from_base_parser<A, B...>::value> {};


        //
        // enable_if_not_same
        //
        // mainly to prevent incident recursive contructor
        // e.g.
        //     struct A {};
        //     struct B : A {
        //         template <typename T>
        //         B(T&& v) : a(make_unique<decay_t<T>>(forward<T>(v)))
        //         {}
        //
        //         unique_ptr<A> a;
        //     };
        //
        //     B x(1);  // T = int
        //     B y = x; // T = B& -> B(B& v) calls make_unique, which in turn calls B(B& v) again
        //
        template <typename A, typename B, typename... C>
        struct enable_if_not_same
            : std::enable_if<false == is_same<typename std::decay<A>::type,
                                              typename std::decay<B>::type,
                                              typename std::decay<C>::type...>::
                                          value> {};
    }
}


namespace spl {
    /**
     * @brief Empty parser, always returns true;
     */
    class empty_parser : public base_parser {
    public:
        virtual bool parse(iterator& first, iterator const& last,
                           error& error) {
            return true;
        }

        virtual char const* name() const { return "empty-parser"; }
    };


    /**
     * @brief End parser, returns true if there is no more character.
     */
    class end_parser : public base_parser {
    public:
        virtual bool parse(iterator& first, iterator const& last,
                           error& error) {
            if (first == last) return true;

            error.message << "failed at end-parser\n";
            error.where = first;
            return false;
        }

        virtual char const* name() const { return "end-parser"; }
    };


    class cstring_parser : public base_parser {
    public:
        cstring_parser(const char* str)
            : m_str(str), m_len(std::strlen(m_str)) {}

        virtual bool parse(iterator& first, iterator const& last,
                           error& error) {
            if (std::distance(first, last) < m_len) {
                error.message << "failed at cstring-parser, text is smaller than expected\n";
                error.where = first;
                return false;
            }

            if (std::equal(m_str, m_str + m_len, first)) {
                std::advance(first, m_len);
                return true;
            }

            error.message << "failed at cstring-parser, needs [" << m_str << "]\n";
            error.where = first;
            return false;
        }

        virtual char const* name() const { return "cstring-parser"; }

    private:
        const char* m_str;
        std::size_t m_len;
    };


    class string_parser : public base_parser {
    public:
        string_parser(std::string str) : m_str(std::move(str)) {}

        virtual bool parse(iterator& first, iterator const& last,
                           error& error) {
            if (std::distance(first, last) < m_str.size()) {
                error.message << "failed at string-parser, text is smaller than expected\n";
                error.where = first;
                return false;
            }

            if (std::equal(m_str.begin(), m_str.end(), first)) {
                std::advance(first, m_str.size());
                return true;
            }

            error.message << "failed at string-parser, needs [" << m_str << "]\n";
            error.where = first;
            return false;
        }

        virtual char const* name() const { return "string-parser"; }

    private:
        std::string m_str;
    };


    class char_parser : public base_parser {
    public:
        char_parser(char c) : m_c(c)
        {}

        virtual bool parse(iterator& first, iterator const& last, error& error) {
            if (std::distance(first, last) < 1) {
                error.message << "failed at char-parser, needs [" << m_c << "], end of text\n";
                error.where = first;
                return false;
            }

            if (*first == m_c) {
                std::advance(first, 1);
                return true;
            }

            error.message << "failed at char-parser, needs [" << m_c  << "]\n";
            error.where = first;
            return false;
        }

        virtual char const* name() const { return "char-parser"; }

    private:
        char m_c;
    };


    class wrapper_parser : public base_parser {
    public:
        template <typename P,
                  typename traits::enable_if_not_same<wrapper_parser, P>::type * = nullptr>
        explicit wrapper_parser(P&& p) :
            m_parser(std::make_shared<typename std::decay<P>::type>(std::forward<P>(p)))
        {}

        virtual bool parse(iterator& first, iterator const& last, error& error) {
            return m_parser->parse(first, last, error);
        }

        wrapper_parser(wrapper_parser const&) = default;
        wrapper_parser(wrapper_parser&&) = default;
        auto operator=(wrapper_parser const&) -> wrapper_parser& = default;
        auto operator=(wrapper_parser&&) -> wrapper_parser& = default;

        template <typename P,
                  typename traits::enable_if_not_same<wrapper_parser, P>::type * = nullptr> 
        auto operator=(P&& p) -> wrapper_parser& {
            m_parser = std::make_shared<typename std::decay<P>::type>(std::forward<P>(p));
            return *this;
        }

        auto parser() const -> std::shared_ptr<base_parser> const& {
            return m_parser;
        }

        virtual char const* name() const { return "wrapper-parser"; }

    public:
        std::shared_ptr<base_parser> m_parser;
    };


    class hook_after_parser : public wrapper_parser {
    public:
        template <typename P>
        explicit hook_after_parser(P&& p, std::function<void()> fn) :
            wrapper_parser(std::forward<P>(p)), m_fn(fn)
        {}

        virtual bool parse(iterator& first, iterator const& last, error& error) {
            if (parser()->parse(first, last, error) == false) {
                error.message << "failed at hook-after-parser\n";
                return false;
            }

            m_fn();
            return true;
        }

        virtual char const* name() const { return "hook-after-parser"; }

    private:
        std::function<void()> m_fn;
    };


    class ignore_parser : public wrapper_parser {
    public:
        template <typename P,
                  typename traits::enable_if_not_same<ignore_parser, P>::type * = nullptr>
        explicit ignore_parser(P&& p) :
            wrapper_parser(std::forward<P>(p))
        {}

        virtual bool parse(iterator& first, iterator const& last, error& error) {
            while (parser()->parse(first, last, error)) {}
            return true;
        }

        virtual char const* name() const { return "ignore-parser"; }
    };


    class xtimes_parser : public wrapper_parser {
    public:
        template <typename P>
        explicit xtimes_parser(P&& p, std::size_t times) :
            wrapper_parser(std::forward<P>(p)), m_times(times)
        {}

        virtual bool parse(iterator& first, iterator const& last, error& error) {
            // quick exit
            if (m_times == 0) return true;

            iterator it = first;
            for (std::size_t i = 0; i < m_times; ++i) {
                if (!parser()->parse(it, last, error)) {
                    error.message << "failed at xtimes-parser, at " << i << ", needs " << m_times << "\n";
                    return false;
                }
            }
            first = it;
            return true;
        }

        virtual char const* name() const { return "xtimes-parser"; }

    private:
        std::size_t m_times;
    };


    class at_least_parser : public xtimes_parser {
    public:
        template <typename P>
        explicit at_least_parser(P&& p, std::size_t least) :
            xtimes_parser(std::forward<P>(p), least)
        {}

        virtual bool parse(iterator& first, iterator const& last, error& error) {
            if (!xtimes_parser::parse(first, last, error)) {
                error.message << "failed at at-least-times\n";
                return false;
            }
            while (parser()->parse(first, last, error)) {}
            return true;
        }

        virtual char const* name() const { return "at-least-parser"; }

    };


    class optional_parser : public wrapper_parser {
    public:
        template <typename P,
                  typename traits::enable_if_not_same<optional_parser, P>::type * = nullptr>
        explicit optional_parser(P&& p) :
            wrapper_parser(std::forward<P>(p))
        {}

        virtual bool parse(iterator& first, iterator const& last, error& error) {
            parser()->parse(first, last, error);
            return true;
        }

        virtual char const* name() const { return "optional-parser"; }
    };


    class group_parser : public base_parser {
    public:
        template <typename... P,
                  typename traits::enable_if_not_same<group_parser, P...>::type * = nullptr>
        explicit group_parser(P&&... p) :
            m_ps{ std::make_shared<typename std::decay<P>::type>(std::forward<P>(p))... }
        {}

        template <typename P>
        void push_back(P&& p) {
            m_ps.push_back(std::make_shared<typename std::decay<P>::type>(std::forward<P>(p)));
        }

        template <typename P>
        void push_front(P&& p) {
            m_ps.emplace(m_ps.begin(), std::make_shared<typename std::decay<P>::type>(std::forward<P>(p)));
        }

        virtual char const* name() const { return "group-parser"; }

    protected:
        auto parsers() const -> std::vector<std::shared_ptr<base_parser>> const& {
            return m_ps;
        }

    private:
        std::vector<std::shared_ptr<base_parser>> m_ps;
    };


    class conjunction_parser : public group_parser {
    public:
        template <typename... P,
                  typename traits::enable_if_not_same<conjunction_parser, P...>::type * = nullptr>
        explicit conjunction_parser(P&&... p) :
            group_parser(std::forward<P>(p)...)
        {}

        virtual bool parse(iterator& first, iterator const& last, error& error) {
            iterator it = first;

            for (auto&& p : parsers()) {
                if (!p->parse(it, last, error)) {
                    error.message << "failed at conjuction-parser\n";
                    return false;
                }
            }

            // only change first if all are true
            first = it;
            return true;
        }

        virtual char const* name() const { return "conjuction-parser"; }
    };



    class disjunction_parser : public group_parser {
    public:
        template <typename... P,
                  typename traits::enable_if_not_same<disjunction_parser, P...>::type * = nullptr>
        explicit disjunction_parser(P&&... p) :
            group_parser(std::forward<P>(p)...)
        {}

        virtual bool parse(iterator& first, iterator const& last, error& error) {
            for (auto&& p : parsers()) {
                if (p->parse(first, last, error)) {
                    return true;
                }
            }

            error.message << "failed at disjunction-parser\n";
            return false;
        }

        virtual char const* name() const { return "disjunction-parser"; }
    };
}


namespace spl {
    namespace operators {
        /**
         * @brief Appear zero or more times.
         *
         * @code
         * // example
         * *p;
         * *(p >> q);
         * @endcode
         */
        template <typename P,
                  typename traits::enable_if_base_from_base_parser<P>::type * = nullptr>
        auto operator*(P&& p) -> at_least_parser {
            return at_least_parser(std::forward<P>(p), 0);
        }


        /**
         * @brief Appear at least once.
         *
         * @code
         * // example
         * +p;
         * +(p >> q);
         * @endcode
         */
        template <typename P,
                  typename traits::enable_if_base_from_base_parser<P>::type * = nullptr>
        auto operator+(P&& p) -> at_least_parser {
            return at_least_parser(std::forward<P>(p), 1);
        }


        /**
         * @brief Appear n times.
         *
         * @code
         * // example
         * p * 5;
         * (p >> q) * 5;
         * @endcode
         */
        template <typename P,
                  typename traits::enable_if_base_from_base_parser<P>::type * = nullptr>
        auto operator*(P&& p, std::size_t times) -> xtimes_parser {
            return xtimes_parser(std::forward<P>(p), times);
        }


        template <typename P,
                  typename traits::enable_if_base_from_base_parser<P>::type * = nullptr>
        auto operator-(P&& p) -> optional_parser {
            return optional_parser(std::forward<P>(p));
        }


        /**
         * @brief Quickly create a conjunction parser.
         *
         * @code
         * // example
         * p >> q;
         * @endcode
         */
        template <typename P1, typename P2,
                  typename traits::enable_if_base_from_base_parser<P1, P2>::type * = nullptr>
        auto operator>>(P1&& lhs, P2&& rhs) -> conjunction_parser {
            return conjunction_parser(std::forward<P1>(lhs), std::forward<P2>(rhs));
        }



        template <typename P,
                  typename traits::enable_if_base_from_base_parser<P>::type * = nullptr>
        auto operator>>(conjunction_parser&& lhs, P&& rhs) -> conjunction_parser&& {
            lhs.push_back(std::forward<P>(rhs));
            return std::move(lhs);
        }


        template <typename P,
                  typename traits::enable_if_base_from_base_parser<P>::type * = nullptr>
        auto operator>>(P&& lhs, conjunction_parser&& rhs) -> conjunction_parser&& {
            rhs.push_front(std::forward<P>(lhs));
            return std::move(rhs);
        }


        /**
         * @brief Quickly create a disjunction parser.
         *
         * @code
         * // example
         * p | q;
         * @endcode
         */
        template <typename P1, typename P2,
                  typename traits::enable_if_base_from_base_parser<P1, P2>::type * = nullptr>
        auto operator|(P1&& lhs, P2&& rhs) -> disjunction_parser {
            return disjunction_parser(std::forward<P1>(lhs), std::forward<P2>(rhs));
        }


        template <typename P,
                  typename traits::enable_if_base_from_base_parser<P>::type * = nullptr>
        auto operator|(disjunction_parser&& lhs, P&& rhs) -> disjunction_parser&& {
            lhs.push_back(std::forward<P>(rhs));
            return std::move(lhs);
        }


        template <typename P,
                  typename traits::enable_if_base_from_base_parser<P>::type * = nullptr>
        auto operator|(P&& lhs, disjunction_parser&& rhs) -> disjunction_parser&& {
            rhs.push_front(std::forward<P>(lhs));
            return std::move(rhs);
        }


        /**
         * @brief Quickly create a conjunction parser with one side is a char.
         *
         * @code
         * // example
         * p >> 'c';
         * 'c' >> p;
         * @endcode
         */
        template <typename P,
                  typename traits::enable_if_base_from_base_parser<P>::type * = nullptr>
        auto operator>>(P&& lhs, char rhs) -> decltype(auto) {
            return lhs >> char_parser(rhs);
        }


        template <typename P,
                  typename traits::enable_if_base_from_base_parser<P>::type * = nullptr>
        auto operator>>(char lhs, P&& rhs) -> decltype(auto) {
            return char_parser(lhs) >> rhs;
        }


        /**
         * @brief Quickly create a conjunction parser with one side is a string.
         *
         * @code
         * // example
         * p >> "string";
         * "string" >> p;
         * p >> std::string("");
         * std::string("") >> p;
         * @endcode
         */
        template <typename P,
                  typename traits::enable_if_base_from_base_parser<P>::type * = nullptr>
        auto operator>>(P&& lhs, char const* rhs) -> decltype(auto) {
            return lhs >> cstring_parser(rhs);
        }


        template <typename P,
                  typename traits::enable_if_base_from_base_parser<P>::type * = nullptr>
        auto operator>>(char const* lhs, P&& rhs) -> decltype(auto) {
            return cstring_parser(lhs) >> rhs;
        }


        template <typename P,
                  typename traits::enable_if_base_from_base_parser<P>::type * = nullptr>
        auto operator>>(P&& lhs, std::string rhs) -> decltype(auto) {
            return lhs >> string_parser(std::move(rhs));
        }


        template <typename P,
                  typename traits::enable_if_base_from_base_parser<P>::type * = nullptr>
        auto operator>>(std::string lhs, P&& rhs) -> decltype(auto) {
            return string_parser(std::move(lhs)) >> rhs;
        }


        /**
         * @brief Quickly create a disjunction parser with one side is a char.
         *
         * @code
         * // example
         * p | 'c';
         * 'c' | p;
         * @endcode
         */
        template <typename P,
                  typename traits::enable_if_base_from_base_parser<P>::type * = nullptr>
        auto operator|(P&& lhs, char rhs) -> decltype(auto) {
            return lhs | char_parser(rhs);
        }


        template <typename P,
                  typename traits::enable_if_base_from_base_parser<P>::type * = nullptr>
        auto operator|(char lhs, P&& rhs) -> decltype(auto) {
            return char_parser(lhs) | rhs;
        }


        /**
         * @brief Quickly create a disjunction parser with one side is a string.
         *
         * @code
         * // example
         * p | "string";
         * "string" | p;
         * p | std::string("");
         * std::string("") | p;
         * @endcode
         */
        template <typename P,
                  typename traits::enable_if_base_from_base_parser<P>::type * = nullptr>
        auto operator|(P&& lhs, char const* rhs) -> decltype(auto) {
            return lhs | cstring_parser(rhs);
        }


        template <typename P,
                  typename traits::enable_if_base_from_base_parser<P>::type * = nullptr>
        auto operator|(char const* lhs, P&& rhs) -> decltype(auto) {
            return cstring_parser(lhs) | rhs;
        }


        template <typename P,
                  typename traits::enable_if_base_from_base_parser<P>::type * = nullptr>
        auto operator|(P&& lhs, std::string rhs) -> decltype(auto) {
            return lhs | string_parser(std::move(rhs));
        }


        template <typename P,
                  typename traits::enable_if_base_from_base_parser<P>::type * = nullptr>
        auto operator|(std::string lhs, P&& rhs) -> decltype(auto) {
            return string_parser(std::move(lhs)) | rhs;
        }
    }
}


#endif // SPL_HPP_INCLUDED

