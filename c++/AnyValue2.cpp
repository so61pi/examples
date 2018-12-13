#include <exception>
#include <iostream>
#include <string>
#include <tuple>
#include <vector>
#include <boost/any.hpp>


enum class AnyType {
    Unknown,
    Boolean,
    Integer,
    Double,
    String
};


using WrappedAny = std::tuple < AnyType, boost::any >;


int main() {
    // push value to vector with its type
    std::vector<WrappedAny> v{};
    v.emplace_back(AnyType::Boolean, true);
    v.emplace_back(AnyType::Integer, -1);
    v.emplace_back(AnyType::Double, 1.5);
    v.emplace_back(AnyType::String, std::string("hello"));

    try {
        // print out values by using embedded type
        for (auto& i : v) {
            auto type = std::get<0>(i);
            auto value = std::get<1>(i);

            switch (type) {
            case AnyType::Boolean:
                std::cout << boost::any_cast<bool>(value) << "\n";
                break;

            case AnyType::Integer:
                std::cout << boost::any_cast<int>(value) << "\n";
                break;

            case AnyType::Double:
                std::cout << boost::any_cast<double>(value) << "\n";
                break;

            case AnyType::String:
                std::cout << boost::any_cast<std::string>(value) << "\n";
                break;

            case AnyType::Unknown:
                std::cout << "Unknown type.\n";
                break;
            }
        }
    }
    catch (std::exception& e) {
        std::cout << e.what() << "\n";
    }
}
