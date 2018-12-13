#include <iostream>
#include <string>
#include <tuple>


int main() {
    auto a = std::make_tuple(true, 1.1, std::string{ "hello" });

    // get number of elements in the tuple
    std::cout << "Size: " << std::tuple_size<decltype(a)>::value << "\n\n";

    // get specific element in the tuple
    std::cout << "First element: " << std::get<0>(a) << "\n";
    std::cout << "Second element: " << std::get<1>(a) << "\n";
    std::cout << "Third element: " << std::get<2>(a) << "\n\n";

    // get the first & third element
    std::tuple_element<0, decltype(a)>::type a0{};
    std::tuple_element<2, decltype(a)>::type a2{};
    std::tie(a0, std::ignore, a2) = a;

    std::cout << "First: " << a0 << "\n"
        << "Third: " << a2 << "\n\n";

    // concatenate several tuples together
    auto b = std::tuple_cat(a, std::make_tuple(static_cast<int *>(nullptr)));
    std::cout << "Size: " << std::tuple_size<decltype(b)>::value << "\n";
}
