#include <iostream>
#include <map>
#include <string>


int main() {
    std::multimap<int, std::string> m{};

    m.emplace(1, "1");
    m.emplace(1, "one");

    m.emplace(2, "2");
    m.emplace(2, "II");
    m.emplace(2, "two");

    // get the number of values associated with the given key
    std::cout << "Key 1 has " << m.count(1) << " value(s).\n";
    std::cout << "Key 2 has " << m.count(2) << " value(s).\n";
    std::cout << "Key 3 has " << m.count(3) << " value(s).\n";

    // get all value associated with particular key
    auto x = m.equal_range(1);
    for (auto i = x.first; i != x.second; ++i)
        std::cout << i->first << " --> " << i->second << "\n";
}
