#include <iostream>


namespace array {


    template<typename T, std::size_t Size>
    class Array {
    public:
        using value_type = T;

        Array() {
            for (auto i = 0; i < Size; ++i) {
                m_e[i] = i;
            }
        }

        value_type operator[](int i) const {
            return m_e[i];
        }

    private: // data
        value_type m_e[Size];
    };


    // expression class
    template<typename LType, typename RType>
    class AAdd {
    public:
        using value_type = typename std::common_type<typename LType::value_type, typename RType::value_type>::type;

        AAdd(const LType& lhs, const RType& rhs) :
            m_lhs(lhs),
            m_rhs(rhs)
        {}

        value_type operator[](int i) const {
            return m_lhs[i] + m_rhs[i];
        }

    private: // data
        const LType& m_lhs;
        const RType& m_rhs;
    };


    template<typename T, std::size_t size>
    AAdd<Array<T, size>, Array<T, size>> operator+(const Array<T, size>& lhs, const Array<T, size>& rhs) {
        // create expression class instead of adding 2 arrays
        return AAdd<Array<T, size>, Array<T, size>>(lhs, rhs);
    }


} // namespace array


int main() {
    array::Array<int, 10> a;
    array::Array<int, 10> b;

    auto r = a + b; // create AAdd expression instead of adding 2 arrays

    auto e = r[0]; // compute a[0] + b[0]
}
