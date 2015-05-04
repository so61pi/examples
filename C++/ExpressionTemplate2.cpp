#include <iostream>
#include <type_traits>


namespace array {


    template<typename T, std::size_t Size>
    class Array {
    public:
        static const std::size_t size = Size;
        using value_type = T;

        Array() {
            for (auto i = 0; i < size; ++i) {
                m_e[i] = i;
            }
        }

        value_type operator[](int i) const {
            return m_e[i];
        }

    private: // data
        value_type m_e[size];
    };


    //
    // expression classes
    //

    template<typename LType, typename RType>
    class AAdd {
    public:
        static_assert(LType::size == RType::size, "Arrays must be same size.");

        static const std::size_t size = LType::size;
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


    template<typename LType, typename RType>
    class ASub {
    public:
        static_assert(LType::size == RType::size, "Arrays must be same size.");

        static const std::size_t size = LType::size;
        using value_type = typename std::common_type<typename LType::value_type, typename RType::value_type>::type;

        ASub(const LType& lhs, const RType& rhs) :
            m_lhs(lhs),
            m_rhs(rhs)
        {}

        value_type operator[](int i) const {
            return m_lhs[i] - m_rhs[i];
        }

    private: // data
        const LType& m_lhs;
        const RType& m_rhs;
    };


    template<typename LType, typename RType>
    class AMul {
    public:
        static_assert(LType::size == RType::size, "Arrays must be same size.");

        static const std::size_t size = LType::size;
        using value_type = typename std::common_type<typename LType::value_type, typename RType::value_type>::type;

        AMul(const LType& lhs, const RType& rhs) :
            m_lhs(lhs),
            m_rhs(rhs)
        {}

        value_type operator[](int i) const {
            return m_lhs[i] * m_rhs[i];
        }

    private: // data
        const LType& m_lhs;
        const RType& m_rhs;
    };


    template<typename LType, typename RType>
    class AScalarMul {
    public:
        static const std::size_t size = RType::size;
        using value_type = typename std::common_type<LType, typename RType::value_type>::type;

        AScalarMul(const LType& lhs, const RType& rhs) :
            m_lhs(lhs),
            m_rhs(rhs)
        {}

        value_type operator[](int i) const {
            return m_lhs * m_rhs[i];
        }

    private: // data
        LType m_lhs;
        const RType& m_rhs;
    };


    template<typename LType, typename RType>
    class AMulScalar {
    public:
        static const std::size_t size = LType::size;
        using value_type = typename std::common_type<typename LType::value_type, RType>::type;

        AMulScalar(const LType& lhs, const RType& rhs) :
            m_lhs(lhs),
            m_rhs(rhs)
        {}

        value_type operator[](int i) const {
            return m_lhs[i] * m_rhs;
        }

    private: // data
        const LType& m_lhs;
        RType m_rhs;
    };


    //
    // operators
    //

    template<typename LType, typename RType>
    AAdd<LType, RType> operator+(const LType& lhs, const RType& rhs) {
        return AAdd<LType, RType>(lhs, rhs);
    }


    template<typename LType, typename RType>
    ASub<LType, RType> operator-(const LType& lhs, const RType& rhs) {
        return ASub<LType, RType>(lhs, rhs);
    }


    // when both LType & RType are NOT scalar types
    // e.g. AAdd<...> * ASub<...>
    template<typename LType, typename RType,
        typename std::enable_if<!std::is_arithmetic<LType>::value>::type * = nullptr,
        typename std::enable_if<!std::is_arithmetic<RType>::value>::type * = nullptr>
    AMul<LType, RType> operator*(const LType& lhs, const RType& rhs) {
        return AMul<LType, RType>(lhs, rhs);
    }


    // when LType is a scalar type
    // e.g. 3 * Array<...>
    template<typename LType, typename RType,
        typename std::enable_if<std::is_arithmetic<LType>::value>::type * = nullptr>
    AScalarMul<LType, RType> operator*(const LType& lhs, const RType& rhs) {
        return AScalarMul<LType, RType>(lhs, rhs);
    }


    // when RType is a scalar type
    // e.g. Array<...> * 3
    template<typename LType, typename RType,
        typename std::enable_if<std::is_arithmetic<RType>::value>::type * = nullptr>
    AMulScalar<LType, RType> operator*(const LType& lhs, const RType& rhs) {
        return AMulScalar<LType, RType>(lhs, rhs);
    }


} // namespace array


int main() {
    array::Array<double, 10> a;
    array::Array<int, 10> b;
    array::Array<int, 10> c;

    // create expressions
    auto r1 = a + b * c;
    auto r2 = a - b + c;
    auto r3 = 3 * r1 + r2 * 2 - r1;

    auto e = r3[0]; // the real computing happens here
}
