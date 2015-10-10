#include <algorithm>
#include <cassert>
#include <iostream>
#include <iterator>
#include <list>
#include <vector>


template<typename ForwardIt1, typename ForwardIt2>
auto permutation_parity(ForwardIt1 first1, ForwardIt1 last1, ForwardIt2 first2, ForwardIt2 last2) -> int {
    assert(std::distance(first1, last1) == std::distance(first2, last2));

    std::vector<bool> marks;
    marks.resize(std::distance(first1, last1));
    int counter = 0;
    bool done = false;
    do {
        // find next unmark pos
        auto unmark = std::find(marks.begin(), marks.end(), false);
        if (unmark == marks.end()) {
            done = true;
        }
        else {
            auto pos1 = std::next(first1, std::distance(marks.begin(), unmark));
            auto f = pos1;
            bool end_cycle = false;
            do {
                // find next pos in the cycle
                // only accept if it is not marked
                bool found = false;
                auto pos2 = first2;
                long long dis = 0;
                do {
                    pos2 = std::find(pos2, last2, *pos1);
                    if (pos2 == last2) return -1;
                    dis = std::distance(first2, pos2);
                    found = (marks[dis] == false);
                    ++pos2;
                } while (!found);

                pos1 = std::next(first1, dis);
                marks[dis] = true;
                if (*f == *pos1) {
                    end_cycle = true;
                }
                else {
                    ++counter;
                }
            } while (!end_cycle);
        }
    } while (!done);

    return counter;
}


int main() {
    std::vector<int> dst{ 1, 2, 3, 4, 5 };

    // 2 move
    std::list<int> src{ 2, 1, 4, 3, 5 };
    std::cout << permutation_parity(cbegin(src), cend(src), cbegin(dst), cend(dst)) << "\n";

    // 0 move
    src = { 1, 2, 3, 4, 5 };
    std::cout << permutation_parity(cbegin(src), cend(src), cbegin(dst), cend(dst)) << "\n";

    // -1 move
    src = { 1, 1, 1, 1, 1 };
    std::cout << permutation_parity(cbegin(src), cend(src), cbegin(dst), cend(dst)) << "\n";
}
