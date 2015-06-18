#include <iostream>
#include <vector>
#include <boost/range/adaptor/reversed.hpp>

/*
Below is the text extracted from Standard Specification.

6.5.4 The range-based for statement [stmt.ranged]
... a range-based for statement is equivalent to

    {
        auto && __range = range-init;
        for (auto __begin = begin-expr, __end = end-expr; __begin != __end; ++__begin) {
            for-range-declaration = *__begin;
            statement
        }
    }
    
...
*/


class my_container {
public:
    my_container()
        : ptr{ new int[10]{0} }
    {
        if (ptr) {
            for (int i = 0; i < 10; ++i)
                ptr[i] = i;
        }
    }
    
    ~my_container() {
        delete ptr;
    }
    
    // used by range based for loop
    auto begin() {
        return ptr;
    }
    
    // used by range based for loop
    auto end() {
        return ptr+10;
    }
    
private:
    int *ptr;
};


int main() {
    // loop through a container
    std::vector<double> v{ 1.0, 2.0, 3.0 };
    for (auto& e : v)
        std::cout << e << " ";
    std::cout << "\n";
    
    // or a braced initializer list
    for (auto& e : { 1, 2, 3 })
        std::cout << e << " ";
    std::cout << "\n";

    // or an array
    const char str[] = "0123456789";
    for (auto& e : str)
        std::cout << e << " ";
    std::cout << "\n";
    
    // and in reverse order
    for (auto& e : boost::adaptors::reverse(str))
        std::cout << e << " ";
    std::cout << "\n";
    
    // can loop through my own class too
    my_container mc;
    for (auto& e : mc)
        std::cout << e << " ";
    std::cout << "\n";
}
