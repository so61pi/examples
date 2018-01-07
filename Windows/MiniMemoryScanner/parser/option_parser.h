#ifndef OPTION_PARSER_H
#define OPTION_PARSER_H


#include <iostream>
#include <string>
#include <boost/optional.hpp>
#include <boost/program_options.hpp>


class option_parser {
public:
    option_parser() :
        m_desc("Options")
    {}


    boost::program_options::options_description_easy_init add_options();
    void parse(int argc, char *argv[]);
    bool empty() const;
    bool exist(std::string const& option) const;


    template<typename T>
    inline boost::optional<T> get(std::string const& option) {
        try {
            return m_vm.at(option).as<T>();
        }
        catch (std::out_of_range const&) {
            return boost::none;
        }
        catch (boost::bad_any_cast const&) {
            return boost::none;
        }
    }


    void print(std::ostream& os) const;


private:
    boost::program_options::options_description m_desc;
    boost::program_options::variables_map m_vm;
};


#endif // OPTION_PARSER_H
