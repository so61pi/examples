#include <algorithm>
#include <functional>
#include <iostream>
#include <iterator>
#include <vector>


template<typename ForwardIt, typename Comparator>
void quick_sort(ForwardIt first, ForwardIt last, Comparator comp) {
    if (first == last) {
        return;
    }

    auto size = std::distance(first, last);
    if (size == 1) {
        return;
    }

    auto middle = std::next(first, size / 2);
    auto pivot = *middle;
    auto m1 = std::partition(first, last, [&](auto& e) { return comp(e, pivot); });
    auto m2 = std::partition(m1, last, [&](auto& e) { return !comp(pivot, e); });
    // y x z 6 6 6 6 b d c a
    // f     1       2     l

    quick_sort(first, m1, comp);
    quick_sort(m2, last, comp);
}


template<typename ForwardIt>
void quick_sort(ForwardIt first, ForwardIt last) {
    quick_sort(first, last, std::less<>{});
}


int main() {
    std::vector<int> data{ 9, 8, 7, 6, 5, 0, 1, 2, 3, 4 };
    quick_sort(begin(data), end(data));

    std::copy(cbegin(data), cend(data), std::ostream_iterator<int>{ std::cout, " " });
}
