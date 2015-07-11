#include <iostream>
#include <string>
#include <boost/type_index.hpp>


template<typename T>
std::string get_type_name() {
    return boost::typeindex::type_id_with_cvr<T>().pretty_name();
}


int main() {
    std::cout << get_type_name<const int *>() << "\n";
    std::cout << get_type_name<std::string&>() << "\n";
}
