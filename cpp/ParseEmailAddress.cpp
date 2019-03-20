#include <complex>
#include <iostream>
#include <string>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/spirit/include/qi.hpp>


namespace qi = boost::spirit::qi;
namespace ascii = boost::spirit::ascii;


struct email {
    std::string name;
    std::string host;
};

BOOST_FUSION_ADAPT_STRUCT(
    email,
    (std::string, name)
    (std::string, host)
    )


template<typename Iterator>
class email_parser : public qi::grammar < Iterator, email() >
{
public:    
    email_parser()
        : email_parser::base_type(start)
    {
        start = name >> '@' >> host;

        name = +(qi::alnum | qi::char_('.') | qi::char_('_'));
        host = +qi::alnum >> +(qi::char_('.') >> +qi::alnum);
        // Cannot define host parser like below
        //
        //   host = +(+qi::alnum >> qi::char_('.')) >> +qi::alnum;
        //
        // The '+(+qi::alnum >> qi::char_('.'))' part will eat all characters.
    }

    qi::rule<Iterator, email()> start;
    qi::rule<Iterator, std::string()> name;
    qi::rule<Iterator, std::string()> host;
};


int main() {
    try {
        std::cout << "Email address parser\n";
        std::cout << "Type 'quit' to quit.\n\n";

        for (std::string line; getline(std::cin, line);) {
            if (line == "quit")
                break;

            email e;

            email_parser<std::string::const_iterator> parser;
            auto result = qi::parse(line.begin(), line.end(), parser, e);
            if (result) {
                std::cout << "Parsing succeeded.\n";
                std::cout
                    << "\tName: " << e.name << "\n" 
                    << "\tDomain: " << e.host << "\n\n";
            }
            else
                std::cout << "Parsing failed.\n\n";
        }
    }
    catch (const std::exception& e) {
        std::cout << e.what();
    }
}
