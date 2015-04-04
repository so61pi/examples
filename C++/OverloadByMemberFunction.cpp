#include <iostream>
#include <type_traits>


class A {
    int m_n = 0;

public:
    A() = default;

    A(int n)
        : m_n{ n }
    {}

    A& plus(const A& rhs) {
        m_n += rhs.m_n;
        return *this;
    }
};


class B {
    int m_n = 0;

public:
    B() = default;

    B(int n)
        : m_n{ n }
    {}

    B& add(const B& rhs) {
        m_n += rhs.m_n;
        return *this;
    }
};


template<
    typename T,
    typename std::enable_if<std::is_member_function_pointer<decltype(&T::plus)>::value>::type * = nullptr >
void DoAdd(T& a, const T& b)
{
    std::cout << "Use plus function.\n";
    a.plus(b);
}


template<
    typename T,
    typename std::enable_if<std::is_member_function_pointer<decltype(&T::add)>::value>::type * = nullptr >
void DoAdd(T& a, const T& b)
{
    std::cout << "Use add function.\n";
    a.add(b);
}


int main() {
    A x(1);
    A y(2);
    DoAdd(x, y);

    B o(1);
    B m(2);
    DoAdd(o, m);
}
