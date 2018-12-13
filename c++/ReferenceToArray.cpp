#include <algorithm>
#include <iostream>
#include <iterator>
#include <type_traits>
#include <boost/mpl/identity.hpp>


namespace mpl = boost::mpl;


// cannot use
// void foo_wrong(int pointer[10]);

void foo_complex(int (&ref_to_array)[10]) {
    for (auto& e : ref_to_array)
        std::cout << e << "\n";
    std::cout << "\n";
}

void foo(mpl::identity<int[10]>::type& ref_to_array) {
    for (auto& e : ref_to_array)
        std::cout << e << "\n";
    std::cout << "\n";
}


// cannot use
// int[10] &bar_wrong(int[10]);

int (&bar_complex(int (&ref_to_array)[10]))[10] {
    foo(ref_to_array);
    return ref_to_array;
}

mpl::identity<int[10]>::type& bar(mpl::identity<int[10]>::type& ref_to_array) {
    foo(ref_to_array);
    return ref_to_array;
}


int main() {
    int a[10];
    std::iota(std::begin(a), std::end(a), 0);
    
    foo(a);
    bar(a);
    
    std::cout << std::is_same<decltype(foo_complex), decltype(foo)>::value << "\n";
    std::cout << std::is_same<decltype(bar_complex), decltype(bar)>::value << "\n";
}
