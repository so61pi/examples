#include <iostream>
#include <string>
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/identity.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/ordered_index.hpp>


struct employee {
    int id;
    std::string name;
    int birth_date;

    employee(int id_, const std::string& name_, int birth_date_)
        : id(id_), name(name_), birth_date(birth_date_)
    {}

    bool operator<(const employee& rhs) const {
        return id < rhs.id;
    }
};


using namespace boost::multi_index;
using employee_set = boost::multi_index_container<
    employee,
    indexed_by<
        ordered_unique<identity<employee>>,
        ordered_non_unique<member<employee, std::string, &employee::name>>,
        ordered_non_unique<member<employee, int, &employee::birth_date>>
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
    for (auto& i : employees.get<1>())
        std::cout << i.id << " " << i.name << " " << i.birth_date << "\n";
    std::cout << "\n";

    // sort by birth_date
    for (auto& i : employees.get<2>())
        std::cout << i.id << " " << i.name << " " << i.birth_date << "\n";
    std::cout << "\n";
}
