#include <iostream>
#include <type_traits>


// #1
template<typename T,
    typename std::enable_if<std::is_integral<T>::value>::type * = nullptr>
        // create nullptr of void * if T is a integral type
        // otherwise create nullptr of <nothing> * -> REMOVE this function from overloading set
void f(const T&) {
    std::cout << "integral type\n";
}


// #2
template<typename T,
    typename = typename std::enable_if<std::is_floating_point<T>::value>::type>
        // create type of void if T is a floating point type
        // otherwise create type of <nothing> -> REMOVE this function from overloading set
void f(const T&) {
    std::cout << "floating point type\n";
}


int main() {
    f(1); // remove #2 due to SFINAE -> call #1

    f(1.0); // remove #1 due to SFINAE -> call #2
}
