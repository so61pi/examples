#include <exception>
#include <iostream>
#include <stdexcept>
#include <string>


int main() {
    try {
        // convert a number to std::string
        auto str = std::to_string(123456789.0);
        std::cout << "str = " << str << "\n";
        
        // convert a std::string to integer
        auto n1 = std::stoi("-123456");
        std::cout << "n1 = " << n1 << "\n";
        
        // ignore prefix white space
        auto n2 = std::stoi("   -123456");
        std::cout << "n2 = " << n2 << "\n";
        
        // with radix
        auto n3 = std::stoi("0x123456", nullptr, 16);
        std::cout << "n3 = " << n3 << "\n";
        
        //
        auto n4 = std::stoi("ZZZ", nullptr, 36);
        std::cout << "n4 = " << n4 << "\n";
        
        // auto detect radix
        auto n5 = std::stoi("0xFF", nullptr, 0);
        std::cout << "n5 = " << n5 << "\n";
        
        // throw an invalid_argument exception when nothing can't be converted
        auto n6 = std::stoi("");
        
        // throw an out_of_range exception when expected converted value is out of range of the result type
        auto n7 = std::stoi("FFFFFFFFFF", nullptr, 16);
    }
    catch (std::invalid_argument const& e) {
        std::cout << "invalid_argument : " << e.what() << "\n";
    }
    catch (std::out_of_range const& e) {
        std::cout << "out_of_range : " << e.what() << "\n";
    }
    catch (std::exception const& e) {
        std::cout << "exception : " << e.what() << "\n";
    }
}
