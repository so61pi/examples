#include <exception>
#include <iostream>
#include <string>
#include <vector>
#include <boost/any.hpp>


int main() {
    std::vector<boost::any> v{};
    v.push_back(1);
    v.push_back(1.0);
    v.push_back("1.0");
    v.push_back(std::string("1.0"));

    // v[0] : int
    // v[1] : double
    // v[2] : const char *
    // v[3] : std::string

    try {
        // get value back, must use exact type
        std::cout << boost::any_cast<int>(v[0]) << "\n";
        std::cout << boost::any_cast<double>(v[1]) << "\n";
        std::cout << boost::any_cast<const char *>(v[2]) << "\n";
        std::cout << boost::any_cast<std::string>(v[3]) << "\n";


        //
        // failure cast
        //

        // return nullptr
        auto ptr = boost::any_cast<char>(&v[0]);
        if (!ptr)
            std::cout << "bad cast\n";

        // throw bad_any_cast
        std::cout << boost::any_cast<char>(v[0]) << "\n";
    }
    catch (std::exception& e) {
        std::cout << e.what() << "\n";
    }
}
