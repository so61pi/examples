#include <iostream>


class my_class {};

std::ostream& operator<<(std::ostream& os, const my_class&) {
    os << "my_class";
    return os;
}


void print(std::ostream& os) {
    os << "recursion end here\n";
}

template<typename FirstType, typename... Args>
void print(std::ostream& os, FirstType ft, Args... args) {
    os << "Number of arguments: " << sizeof...(args) << "\n";
    
    // print arguments in reverse order
    print(os, args...);
    os << ft;
}


int main() {
    print(std::cout, 1, "2", 3.0, my_class{});
}
