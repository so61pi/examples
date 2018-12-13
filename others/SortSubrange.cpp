#include <algorithm>
#include <cassert>
#include <functional>
#include <iostream>
#include <iterator>
#include <vector>


template<typename RandomIt, typename Comparator>
void sort_subrange(RandomIt first, RandomIt last, RandomIt sub_first, RandomIt sub_last, Comparator comp) {
    assert(first <= sub_first && sub_first <= sub_last && sub_last <= last);

    if (sub_first < sub_last) {
        std::nth_element(first, sub_first, last, comp);
        std::partial_sort(std::next(sub_first), sub_last, last, comp);
    }
}


template<typename RandomIt>
void sort_subrange(RandomIt first, RandomIt last, RandomIt sub_first, RandomIt sub_last) {
    sort_subrange(first, last, sub_first, sub_last, std::less<>{});
}


int main() {
    std::vector<int> data{ 9, 5, 7, 3, 8, 6, 2, 4, 1, 0 };
    //                           f        l
    //                           2  3  4  5
    sort_subrange(begin(data), end(data), begin(data) + 2, begin(data) + 5);

    std::copy(cbegin(data), cend(data), std::ostream_iterator<int>{ std::cout, " " });
}
