#include <iostream>
#include <string>
#include <boost/multi_index/identity.hpp>
#include <boost/multi_index/mem_fun.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/tag.hpp>
#include <boost/multi_index_container.hpp>


struct employee {
    int id;
    std::string name;
    int birth_date;

    employee(int id_, const std::string& name_, int birth_date_)
        : id(id_), name(name_), birth_date(birth_date_)
    {}

    std::size_t name_length() const {
        return name.size();
    }

    // MultiIndex uses operator less than by default (std::less)
    bool operator<(const employee& rhs) const {
        return id < rhs.id;
    }
};


// tags
struct name_tag {};
struct birth_date_tag {};
struct name_length_tag {};


using namespace boost::multi_index;
using employee_set = boost::multi_index_container<
    employee,
    indexed_by<
        ordered_unique<identity<employee>>,
        ordered_non_unique<tag<name_tag>, member<employee, std::string, &employee::name>, std::greater<std::string>>,
        ordered_non_unique<tag<birth_date_tag>, member<employee, int, &employee::birth_date>>,
        ordered_non_unique<tag<name_length_tag>, const_mem_fun<employee, std::size_t, &employee::name_length>>
    >>;


int main() {
    employee_set employees{};

    // insert elements
    employees.emplace(1, "Einstein", 1879);
    employees.emplace(2, "Edison", 1847);
    employees.emplace(3, "Washington", 1732);
    employees.emplace(4, "Andrew", 1960);
    employees.emplace(5, "Zorro", -1);
    employees.emplace(6, "Anderson", 1836);

    // sort by id (default)
    for (auto& i : employees)
        std::cout << i.id << " " << i.name << " " << i.birth_date << "\n";
    std::cout << "\n";

    // sort by name
    for (auto& i : employees.get<name_tag>())
        std::cout << i.id << " " << i.name << " " << i.birth_date << "\n";
    std::cout << "\n";

    // sort by birth_date
    for (auto& i : employees.get<birth_date_tag>())
        std::cout << i.id << " " << i.name << " " << i.birth_date << "\n";
    std::cout << "\n";

    // sort by name length
    for (auto& i : employees.get<name_length_tag>())
        std::cout << i.id << " " << i.name << " " << i.birth_date << "\n";
    std::cout << "\n";
}
