#ifndef PATTERN_PARSER_H
#define PATTERN_PARSER_H


#include <cstdint>
#include <string>
#include <vector>
#include <boost/optional.hpp>


std::vector<boost::optional<std::uint8_t>> parse_pattern(std::string const& pattern);


#endif // PATTERN_PARSER_H
