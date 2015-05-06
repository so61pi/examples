#include <iostream>
#include <type_traits>


// #1
template<typename T,
    typename std::enable_if<std::is_integral<T>::value>::type * = nullptr>
        // create nullptr of void * if T is a integral type
        // otherwise remove this function from overloading set
        //
        // use enable_if this way instead of #2 way
void f(const T&) {
    std::cout << "integral type\n";
}


// #2
template<typename T,
    typename = typename std::enable_if<std::is_floating_point<T>::value>::type>
        // create type of void if T is a floating point type
        // otherwise remove this function from overloading set
        //
        // but don't use enable_if this way, because if the user explicitly
        // provides template arguments like this
        //     f<void *, whatever-type>(nullptr);
        // then the enable_if part is ignored!!!
void f(const T&) {
    std::cout << "floating point type\n";
}


int main() {
    f(1); // remove #2 due to SFINAE -> call #1

    f(1.0); // remove #1 due to SFINAE -> call #2

    f<void *, int>(nullptr); // horrible thing happens here!!!
}
