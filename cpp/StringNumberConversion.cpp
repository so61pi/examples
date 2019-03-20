#include <algorithm>
#include <exception>
#include <ios>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>


template<typename CharType = char, typename T>
auto to_hex_string(T const& value, bool upper = true, bool showbase = true, bool upper_base = false) {
    using string_type = std::basic_string<CharType>;
    using stream_type = std::basic_stringstream<CharType>;
    
    string_type str;
    stream_type ss;
    if (upper)
        ss << std::hex << std::uppercase << value;
    else
        ss << std::hex << value;
    ss >> str;
    
    if (showbase) {
        std::reverse(str.begin(), str.end());
        str.push_back(upper_base ? CharType('X') : CharType('x'));
        str.push_back(CharType('0'));
        std::reverse(str.begin(), str.end());
    }

    return str;
}


int main() {
    try {
        // convert a number to std::string
        auto s1 = std::to_string(123456789.0);
        std::cout << "s1 = " << s1 << "\n";
        
        // convert to hex format
        auto s2 = to_hex_string(127);
        std::cout << "s2 = " << s2 << "\n";
        
        auto s3 = to_hex_string<wchar_t>(127);
        std::wcout << L"s3 = " << s3 << L"\n";
        
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
