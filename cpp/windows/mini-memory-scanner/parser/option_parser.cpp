#include "option_parser.h"


boost::program_options::options_description_easy_init option_parser::add_options() {
    return m_desc.add_options();
}


void option_parser::parse(int argc, char *argv[]) {
    boost::program_options::store(boost::program_options::parse_command_line(argc, argv, m_desc), m_vm);
    boost::program_options::notify(m_vm);
}


bool option_parser::empty() const {
    return m_vm.empty();
}


bool option_parser::exist(std::string const& option) const {
    return (m_vm.count(option) != 0);
}


void option_parser::print(std::ostream& os) const {
    m_desc.print(os);
}
