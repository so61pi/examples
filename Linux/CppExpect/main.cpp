#include <exception>
#include <iostream>
#include <iterator>
#include <regex>
#include <stdexcept>
#include <string>
#include <vector>

#include "Expect.h"


///
/// \brief Get a sub-string of str base on regex pattern.
/// \param str
/// \param pattern
/// \return
///
auto PatternFind(std::string const& str, std::string const& pattern)
    -> std::string {
    std::regex pat{pattern};
    auto       begin = std::sregex_iterator{str.begin(), str.end(), pat};
    auto       end   = std::sregex_iterator{};
    for (auto i = begin; i != end; ++i) {
        std::smatch match = *i;
        return match.str();
    }
    return "";
}


///
/// \brief Get a sub-string of str base on multiple nested patterns.
/// \param str
/// \param pat
/// \param subpat
/// \param patterns
/// \return
///
/// \code
///     // example
///     auto string = "First value  : 500\n"
///                   "Second value : 1000\n"
///                   "Third value  : I'm a string\n"
///     auto value = PatternFind(string, {"Second value : [0-9]+", "[0-9]+"});
///     // value = 1000
/// \endcode
auto MultiPatternFind(std::string const&              str,
                      std::vector<std::string> const& patterns) {
    auto sub = str;
    for (auto const& pattern : patterns) {
        sub = PatternFind(sub, pattern);
    }
    return sub;
}


///
/// \brief Split
/// \param str
/// \param delim
///
auto Split(std::string const& str, char delim) {
std::vector<std::string> elements;
    std::string::size_type begin = 0, end = 0;

    while (true) {
        begin = str.find_first_not_of(delim, end);
        if (begin == std::string::npos) return elements;

        end = str.find_first_of(delim, begin);
        if (end == std::string::npos) end = str.size();

        elements.emplace_back(str.begin() + begin, str.begin() + end);
    }

    return elements;
}


int main([[maybe_unused]] int argc, char* argv[]) {
    using expect::re;
    try {
        auto uid = ":~\\$ " + re;
        expect::ExpectProcess a{"/usr/bin/ssh", argv};
        bool login = false;
        while (!login) {
            auto status = a.Expect({"(yes/no)", "(P|p)assword:"+re, uid});
            switch (status) {
            case 0:
                a.Send("yes\n");
                break;

            case 1:
                a.Send("vagrant\n");
                break;

            case 2:
                login = true;
                break;

            default:
                throw std::runtime_error{expect::Status::ToString(status)};
            }
        }

        a.Send("ifconfig\n");
        a.Expect(uid);
        auto ifInfo = a.LastOutput();
        a.Send("exit\n");

        std::cout << "MAC : " << MultiPatternFind(ifInfo, {R"(eth0(.+\r\n)+\r\n)", "HWaddr ([0-9a-f]{2}:){5}[0-9a-f]{2}", "([0-9a-f]{2}:){5}[0-9a-f]{2}"}) << "\n";
    } catch (std::exception const& e) {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}
