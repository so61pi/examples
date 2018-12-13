#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>
#include "grammar.hpp"


namespace qi = boost::spirit::qi;


extern template
class c_grammar<std::vector<char>::iterator, qi::unused_type, qi::space_type>;

extern template
c_grammar<std::vector<char>::iterator, qi::unused_type, qi::space_type>::c_grammar();

extern template
c_grammar<std::vector<char>::iterator, qi::unused_type, qi::space_type>::~c_grammar();

extern template
bool boost::spirit::qi::phrase_parse(
    std::vector<char>::iterator const&,
    std::vector<char>::iterator,
    c_grammar<std::vector<char>::iterator, qi::unused_type, qi::space_type> const&,
    qi::space_type const&,
    BOOST_SCOPED_ENUM(qi::skip_flag));


int main(int argc, char *argv[]) {
    try {
        if (argc < 2) {
            std::cout << "Please supply a C source file.\n";
            return 0;
        }

        std::ifstream file(argv[1]);
        file >> std::noskipws;
        if (file) {
            std::vector<char> v;
            std::copy(
                std::istream_iterator<char>(file),
                std::istream_iterator<char>(),
                std::back_inserter(v));

            c_grammar<std::vector<char>::iterator, qi::unused_type, qi::space_type> parser;

            auto result = qi::phrase_parse(v.begin(), v.end(), parser, qi::space);

            std::cout << "Result: " << result << "\n";
        }
        else {
            std::cout << "Cannot open file.\n";
        }
    }
    catch (const std::exception& e) {
        std::cout << e.what();
    }
}
