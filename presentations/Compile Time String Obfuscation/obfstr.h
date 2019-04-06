#ifndef OBFSTR_H
#define OBFSTR_H


namespace obfstr {

    namespace detail {
        template<typename T>
        struct identity {
            using type = T;
        };
    }


    /*
    RSA

    Public Key:
    n = 667
    e = 3

    Private Key:
    n = 667
    d = 411
    */


    template<int NKey, int EKey, int DKey>
    struct rsa {
        static constexpr int encrypt(char c) {
            int result = 1;
            for (auto i = 0; i < EKey; ++i) {
                result *= c;
                result %= NKey;
            }
            return result;
        }

        static constexpr char decrypt(int c) {
            int result = 1;
            for (auto i = 0; i < DKey; ++i) {
                result *= c;
                result %= NKey;
            }
            return result;
        }
    };


    template<int N>
    struct obstring {
        constexpr obstring(typename detail::identity<char const[N]>::type& p) :
            m_str{ 0 }
        {
            for (auto i = 0; i < N; ++i) {
                m_str[i] = rsa<667, 3, 411>::encrypt(p[i]);
            }
        }

        constexpr auto get() const noexcept -> decltype(auto) {
            return (m_str);
        }

        constexpr auto size() const noexcept -> int {
            return N;
        }

        int m_str[N];
    };


    template<int N>
    struct unobstring {
        unobstring(typename detail::identity<int const[N]>::type& p) :
            m_str{ 0 }
        {
            for (auto i = 0; i < N; ++i) {
                m_str[i] = rsa<667, 3, 411>::decrypt(p[i]);
            }
        }

        auto get() const noexcept -> decltype(auto) {
            return (m_str);
        }

        char m_str[N];
    };
}


#ifdef NDEBUG
#   define OBFSTR(STR)                                              \
        (                                                           \
            []() {                                                  \
                constexpr obfstr::obstring<sizeof(STR)> a{ STR };   \
                return obfstr::unobstring<a.size()>{ a.get() };     \
            }                                                       \
        )().get()
#else
#   define OBFSTR(STR) STR
#endif


#endif // OBFSTR_H
