#include <algorithm>
#include <iostream>
#include <string>
#include <boost/bimap.hpp>


int main() {
    using bm_type = boost::bimap<int, std::string>;
    using bm_value_type = bm_type::value_type;

    // create a simple bimap
    // both sides (left + right) are unique keys
    bm_type m{};

    // insert elements
    m.insert({ 1, "one" });
    m.insert({ 2, "two" });
    m.insert({ 3, "three" });
    m.insert({ 4, "four" });
    m.insert({ 5, "five" });
    m.insert({ 6, "six" });
    m.insert({ 7, "seven" });
    m.insert({ 8, "eight" });
    m.insert({ 9, "nine" });
    m.insert({ 0, "zero" });
    m.insert({ 0, "0" }); // has no effect because both sides are unique keys
                          // { 0, "zero" } remains unchanged

    // get the number of relations
    std::cout << "Number of relations: " << m.size() << "\n\n";

    // loop through all relations
    std::cout << "All relations\n";
    for (auto& i : m)
        std::cout << "\t" << i.get_left() << " <--> " << i.get_right() << "\n";
    std::cout << "\n";

    // find a specific value
    //
    // auto pos = m.find(bm_value_type{ 1, "one" });
    auto pos = std::find(m.begin(), m.end(), bm_value_type{ 1, "one" });
    if (pos != m.end())
        std::cout << "Found: " << pos->get_left() << " <--> " << pos->get_right() << "\n\n";
    else
        std::cout << "Key is not found.\n\n";


    //
    // left view
    //
    std::cout << "Left view\n";
    for (auto& i : m.left)
        std::cout << "\t" << i.first << " --> " << i.second << "\n";

    // find value from key
    std::cout << m.left.find(1)->second << "\n";
    std::cout << m.left.at(2) << "\n\n"; // cannot use m.left[2]


    //
    // right view
    //
    std::cout << "Right view\n";
    for (auto& i : m.right)
        std::cout << "\t" << i.first << " --> " << i.second << "\n";

    // find value from key
    std::cout << m.right.find("three")->second << "\n";
    std::cout << m.right.at("four") << "\n\n"; // cannot use m.right["four"]
}
