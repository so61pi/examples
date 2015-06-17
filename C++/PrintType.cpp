#include <iostream>
#include <string>
#include <typeinfo>


#ifdef _MSC_VER

template<typename T>
std::string get_type() {
    T* temp = nullptr;
    return typeid(*temp).name();
}

#elif defined(__GNUC__)

#include <cxxabi.h>

template<typename T>
std::string get_type() {
    int status = -1;
    
    T* temp = nullptr;
    return abi::__cxa_demangle(typeid(*temp).name(), nullptr, nullptr, &status);
}

#endif


int main() {
    std::cout << get_type<const int *>() << "\n";
    std::cout << get_type<std::string *>() << "\n";
}
