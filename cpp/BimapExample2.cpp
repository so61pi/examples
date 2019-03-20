#include <algorithm>
#include <iostream>
#include <string>
#include <boost/bimap.hpp>
#include <boost/bimap/set_of.hpp>
#include <boost/bimap/multiset_of.hpp>


int main() {
    using bm_type = boost::bimap<
        boost::bimaps::set_of<std::string>,
        boost::bimaps::multiset_of<long long>>;
    using bm_value_type = bm_type::value_type;

    // insert elements
    // { country, population }
    bm_type m{};
    m.insert({ "Brazil",        186500000 });
    m.insert({ "Pakistan",      163630000 });
    m.insert({ "China",         1321000000 });
    m.insert({ "Indonesia",     234950000 });
    m.insert({ "India",         1129000000 });
    m.insert({ "United States", 301950000 });

    // get the population of Indonesia
    std::cout << "Population of Indonesia is " << m.left.at("Indonesia") << "\n";

    // find the country whose population is bigger than India
    auto left = m.left.find("India");   // get left -> right iterator
    auto right = m.project_right(left); // change to right -> left iterator
    ++right;                            // get next bigger population
    std::cout << right->second << "\n"; // get the country name
}
