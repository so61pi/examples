#include <iostream>
#include <iterator>
#include <map>
#include <string>
#include <vector>

//#define BOOST_SPIRIT_X3_DEBUG

#include <boost/fusion/include/std_pair.hpp>
#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/home/x3/support/ast/variant.hpp>


using namespace boost::spirit::x3;


namespace json {

    using string_t = std::string;
    using number_t = double;
    using bool_t   = bool;
    class null_t {};
    class value_t;
    using pair_t   = std::pair<std::string, value_t>;
    using object_t = std::vector<pair_t>;
    using array_t  = std::vector<value_t>;


    class value_t : public variant<null_t, bool_t, number_t, string_t, object_t,
                                   array_t> {
    public:
        using value_type = value_t;

        using base_type::base_type;
        using base_type::operator=;

        value_t() : base_type{null_t{}} {}
        value_t(char const* val) : base_type{string_t{val}} {}
    };


    namespace parser {

        // use "elaborated type specifier" syntax
        using value_type  = rule<class value_class, value_t>;
        using pair_type   = rule<class pair_class, pair_t>;
        using object_type = rule<class object_class, object_t>;
        using array_type  = rule<class array_class, array_t>;

        value_type const value   = "value";
        pair_type const pair     = "pair";
        object_type const object = "object";
        array_type const array   = "array";

        auto const string_def = lexeme['"' >> *(char_ - '"') >> '"'];
        auto const number_def = double_;
        auto const bool_def   = bool_;
        auto const null_def   = "null" >> attr(null_t{});
        auto const value_def =
              null_def
            | bool_def
            | number_def
            | string_def
            | object
            | array;
        auto const pair_def   = string_def >> ':' >> value;
        auto const object_def = '{' >> -(pair % ",") >> '}';
        auto const array_def  = '[' >> -(value % ",") >> ']';

        BOOST_SPIRIT_DEFINE(value, pair, object, array)
    }

}


struct printer_visitor : boost::static_visitor<> {
    void operator()(json::null_t) {
        std::cout << std::string(m_align, ' ') << "null"
                  << "\n";
    }
    void operator()(json::bool_t const& val) {
        std::cout << std::string(m_align, ' ') << val << "\n";
    }
    void operator()(json::number_t const& val) {
        std::cout << std::string(m_align, ' ') << val << "\n";
    }
    void operator()(json::string_t const& val) {
        std::cout << std::string(m_align, ' ') << '"' << val << '"' << "\n";
    }

    void operator()(json::object_t const& val) {
        std::cout << std::string(m_align, ' ') << "<object>\n";
        m_align += 4;
        for (auto const& pair : val) {
            std::cout << std::string(m_align, ' ') << "<" << pair.first << ">"
                      << "\n";
            m_align += 4;
            boost::apply_visitor(*this, pair.second);
            m_align -= 4;
            std::cout << std::string(m_align, ' ') << "</" << pair.first << ">"
                      << "\n";
        }
        m_align -= 4;
        std::cout << std::string(m_align, ' ') << "</object>\n";
    }

    void operator()(json::array_t const& val) {
        std::cout << std::string(m_align, ' ') << "<array>\n";
        m_align += 4;
        for (auto const& e : val) {
            boost::apply_visitor(*this, e);
        }
        m_align -= 4;
        std::cout << std::string(m_align, ' ') << "</array>\n";
    }

    std::size_t m_align = 0;
};


struct compact_printer_visitor : boost::static_visitor<> {
    void operator()(json::null_t) const { std::cout << "null"; }
    void operator()(json::bool_t const& val) const { std::cout << val; }
    void operator()(json::number_t const& val) const { std::cout << val; }
    void operator()(json::string_t const& val) const {
        std::cout << '"' << val << '"';
    }

    void operator()(json::object_t const& val) const {
        std::cout << "{";
        for (auto it = cbegin(val), eit = cend(val); it != eit; ++it) {
            std::cout << '"' << it->first << '"' << ":";
            boost::apply_visitor(*this, it->second);
            if (std::next(it) != eit) std::cout << ",";
        }
        std::cout << "}";
    }

    void operator()(json::array_t const& val) const {
        std::cout << "[";
        for (auto it = cbegin(val), eit = cend(val); it != eit; ++it) {
            boost::apply_visitor(*this, *it);
            if (std::next(it) != eit) std::cout << ",";
        }
        std::cout << "]";
    }
};


struct pretty_printer_visitor : boost::static_visitor<> {
    void operator()(json::null_t) { std::cout << "null"; }
    void operator()(json::bool_t const& val) { std::cout << val; }
    void operator()(json::number_t const& val) { std::cout << val; }
    void operator()(json::string_t const& val) {
        std::cout << '"' << val << '"';
    }

    void operator()(json::object_t const& val) {
        if (val.empty()) {
            std::cout << "{}";
            return;
        }

        std::cout << "{\n";
        m_align += 4;
        for (auto it = cbegin(val), eit = cend(val); it != eit; ++it) {
            std::cout << std::string(m_align, ' ') << '"' << it->first << '"'
                      << " : ";
            boost::apply_visitor(*this, it->second);
            if (std::next(it) != eit)
                std::cout << ",\n";
            else
                std::cout << "\n";
        }
        m_align -= 4;
        std::cout << std::string(m_align, ' ') << "}";
    }

    void operator()(json::array_t const& val) {
        if (val.empty()) {
            std::cout << "[]";
            return;
        }

        std::cout << "[\n";
        m_align += 4;
        for (auto it = cbegin(val), eit = cend(val); it != eit; ++it) {
            std::cout << std::string(m_align, ' ');
            boost::apply_visitor(*this, *it);
            if (std::next(it) != eit)
                std::cout << ",\n";
            else
                std::cout << "\n";
        }
        m_align -= 4;
        std::cout << std::string(m_align, ' ') << "]";
    }

    std::size_t m_align = 0;
};


int main() {
    char const str[] =
R"({"firstName": "John",
"lastName" : "Smith",
"isAlive" : true,
"age" : 25,
"address" : {
    "streetAddress": "21 2nd Street",
    "city" : "New York",
    "state" : "NY",
    "postalCode" : "10021-3100"
},
"phoneNumbers": [
    {
        "type": "home",
        "number" : "212 555-1234"
    },
    {
        "type": "office",
        "number" : "646 555-4567"
    }
],
    "children": [],
    "spouse" : null
})";

    try {
        json::value_t value;
        auto success = phrase_parse(std::cbegin(str), std::cend(str),
                                    json::parser::value, space, value);
        if (success) {
            pretty_printer_visitor printer;
            boost::apply_visitor(printer, value);
        } else {
            std::cout << "parse failed\n";
        }
    } catch (std::exception const& e) {
        std::cout << e.what() << '\n';
    }
}
