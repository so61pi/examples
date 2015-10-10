#include <algorithm>
#include <iostream>
#include <iterator>
#include <utility>
#include <vector>


template<typename It, typename BinaryPred>
auto group_range(It first, It last, BinaryPred pred) -> std::vector<std::pair<It, It>> {
    std::vector<std::pair<It, It>> result;

    auto not2 = [pred](auto const& a, auto const& b) -> bool {
        return !pred(a, b);
    };

    while (first != last) {
        auto found = std::adjacent_find(first, last, not2);
        if (found != last) {
            std::advance(found, 1);
        }
        result.emplace_back(first, found);
        first = found;
    }

    return result;
}


template<typename It, typename UnaryPred>
auto group_point(It first, It last, UnaryPred pred) -> std::vector<std::pair<It, It>> {
    std::vector<std::pair<It, It>> result;

    while (first != last) {
        auto found = std::find_if(first, last, pred);
        if (found != last) {
            std::advance(found, 1);
        }
        result.emplace_back(first, found);
        first = found;
    }

    return result;
}


int main() {
    std::vector<int> a{ 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
    auto g1 = group_point(begin(a), end(a), [](auto const& e) { return e % 7 == 0; });
    for (auto const& g : g1) {
        std::copy(g.first, g.second, std::ostream_iterator<int>{ std::cout, " " });
        std::cout << "\n";
    }
    std::cout << "\n";

    std::vector<int> b{ 3, 3, 5, 5, 5, 100, 8, 100, 10, 12, 12, 12 };
    std::sort(begin(b), end(b));
    auto g2 = group_range(begin(b), end(b), [](auto const& a, auto const& b) { return a == b; });
    for (auto const& g : g2) {
        std::copy(g.first, g.second, std::ostream_iterator<int>{ std::cout, " " });
        std::cout << "\n";
    }
    std::cout << "\n";
}
