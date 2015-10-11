#include <algorithm>
#include <cstddef>
#include <functional>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>


template<typename T, typename U>
auto build_matrix(std::size_t rows, std::size_t cols, U&& defval) {
    std::vector<std::vector<T>> matrix;
    matrix.resize(rows);
    for (auto& row : matrix) {
        row.resize(cols, std::forward<U>(defval));
    }
    return matrix;
};


template<typename RandomIt1, typename RandomIt2, typename EqualComparator>
auto levenshtein_distance(RandomIt1 src_first, RandomIt1 src_last, RandomIt2 dst_first, RandomIt2 dst_last, EqualComparator ecomp) -> int {
    auto src_size = std::distance(src_first, src_last);
    auto dst_size = std::distance(dst_first, dst_last);
    auto matrix = build_matrix<int>(dst_size + 1, src_size + 1, 0);

    for (auto c = 0; c < src_size + 1; ++c)
        matrix[0][c] = c;
    for (auto r = 0; r < dst_size + 1; ++r)
        matrix[r][0] = r;

    for (auto r = 1; r < dst_size + 1; ++r) {
        for (auto c = 1; c < src_size + 1; ++c) {
            auto cost = !ecomp(src_first[c - 1], dst_first[r - 1]) ? 1 : 0;
            matrix[r][c] =
                std::min({ matrix[r - 1][c - 1] + cost, matrix[r][c - 1] + 1, matrix[r - 1][c] + 1 });
        }
    }

    return matrix[dst_size][src_size];
}


template<typename RandomIt1, typename RandomIt2>
auto levenshtein_distance(RandomIt1 src_first, RandomIt1 src_last, RandomIt2 dst_first, RandomIt2 dst_last) {
    return levenshtein_distance(src_first, src_last, dst_first, dst_last, std::equal_to<>{});
}


int main() {
    std::string s1{ "Huynh Ngoc Thi" };
    std::string s2{ "so61pi" };
    std::cout << levenshtein_distance(cbegin(s1), cend(s1), cbegin(s2), cend(s2)) << '\n';

    std::vector<int> dt1{ 10, 11 };
    std::vector<int> dt2{ 1, 2, 3, 4, 5 };
    std::cout << levenshtein_distance(cbegin(dt1), cend(dt1), cbegin(dt2), cend(dt2)) << '\n';
}
