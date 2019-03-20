#include "pattern_parser.h"

#include <cctype>
#include <stdexcept>


std::vector<boost::optional<std::uint8_t>> parse_pattern(std::string const& pattern) {
    auto xdigit_to_num = [](std::uint8_t digit) -> std::uint8_t {
        digit = std::toupper(digit);
        if ('A' <= digit && digit <= 'F')
            return digit - 'A' + 0x0A;
        else
            return digit - '0' + 0;
    };

    std::vector<boost::optional<std::uint8_t>> result;

    int counter = 0;
    for (auto& e : pattern) {
        if (std::isspace(e)) {
            continue;
        }
        else if (e == '?') {
            switch (counter % 2) {
            case 0:
                result.emplace_back(boost::none);
                break;

            case 1:
                if (result.back()) // e.g 00 1? 22
                    throw std::runtime_error("invalid pattern");
                else
                    break;
                break;
            }

            ++counter;
        }
        else if (std::isxdigit(e)) {
            switch (counter % 2) {
            case 0:
                result.emplace_back(xdigit_to_num(e));
                break;

            case 1:
                if (result.back())
                    result.back() = (*result.back()) * 16 + xdigit_to_num(e);
                else // e.g 00 ?1 22
                    throw std::runtime_error("invalid pattern");

                break;
            }

            ++counter;
        }
        else
            throw std::runtime_error("invalid pattern");
    }
    if (counter % 2 != 0) // e.g. 00 11 2
        throw std::runtime_error("invalid pattern");

    return result;
}
