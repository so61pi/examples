#include <algorithm>
#include <functional>
#include <iostream>
#include <iterator>
#include <vector>


template<typename BidirIt, typename Comparator>
void merge_sort(BidirIt first, BidirIt last, Comparator comp) {
    if (first == last) {
        return;
    }

    auto size = std::distance(first, last);
    if (size == 1) {
        return;
    }

    auto middle = std::next(first, size / 2);
    merge_sort(first, middle, comp);
    merge_sort(middle, last, comp);
    std::inplace_merge(first, middle, last);
}


template<typename BidirIt>
void merge_sort(BidirIt first, BidirIt last) {
    merge_sort(first, last, std::less<>{});
}


int main() {
    std::vector<int> data{ 9, 8, 7, 6, 5, 0, 1, 2, 3, 4 };
    merge_sort(begin(data), end(data));

    std::copy(cbegin(data), cend(data), std::ostream_iterator<int>{ std::cout, " " });
}
