#include <complex>
#include <iostream>
#include <string>
#include <boost/spirit/include/qi.hpp>


// parse complex number in the form of a + bi
bool parse_complex_number(const std::string& str, std::complex<double>& val) {
    namespace qi = boost::spirit::qi;
    namespace ascii = boost::spirit::ascii;

    using qi::double_;
    using qi::phrase_parse;
    using ascii::space;

    // construct the grammar
    auto real_part = double_;
    auto imag_part = double_ >> 'i';
    auto grammar = real_part >> -imag_part;

    double real = 0;
    double imag = 0;
    auto begin = str.begin();
    auto end = str.end();
    bool r = phrase_parse(begin, end, grammar, space, real, imag);

    // fail if we did not get a full match
    if (begin != end)
        return false;

    // set the value if success
    val = { real, imag };
    return r;
}


int main() {
    std::cout << "Complex number parser.\n";
    std::cout << "Type 'quit' to quit.\n\n";

    for (std::string line; getline(std::cin, line);) {
        if (line == "quit")
            break;

        std::complex<double> val;
        if (parse_complex_number(line, val)) {
            std::cout << "Parsing succeeded.\n";
            std::cout << "You entered (" << val.real() << "; " << val.imag() << ")\n\n";
        }
        else {
            std::cout << "Parsing failed\n\n";
        }
    }
}
