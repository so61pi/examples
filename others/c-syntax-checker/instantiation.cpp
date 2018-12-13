#include "grammar.cpp.hpp"


namespace qi = boost::spirit::qi;


template
class c_grammar<std::vector<char>::iterator, qi::unused_type, qi::space_type>;

template
c_grammar<std::vector<char>::iterator, qi::unused_type, qi::space_type>::c_grammar();

template
c_grammar<std::vector<char>::iterator, qi::unused_type, qi::space_type>::~c_grammar();

template
bool boost::spirit::qi::phrase_parse(
    std::vector<char>::iterator const&,
    std::vector<char>::iterator,
    c_grammar<std::vector<char>::iterator, qi::unused_type, qi::space_type> const&,
    qi::space_type const&,
    BOOST_SCOPED_ENUM(qi::skip_flag));
