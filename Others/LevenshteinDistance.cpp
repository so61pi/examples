#include <algorithm>
#include <cstddef>
#include <iostream>
#include <iterator>
#include <string>
#include <tuple>
#include <utility>
#include <vector>


template <typename T, typename U>
auto build_matrix(std::size_t rows, std::size_t cols, U&& defval) {
    std::vector<std::vector<T>> matrix;
    matrix.resize(rows);
    for (auto& row : matrix) {
        row.resize(cols, std::forward<U>(defval));
    }
    return matrix;
}


enum class edit { nothing, substitute, insert, remove };


template <typename Matrix>
auto levenshtein_distance_edit_path(Matrix const& matrix, std::size_t rows,
                                    std::size_t cols) -> std::vector<edit> {
    // first = row, second = column
    using pos_type = std::pair<std::size_t, std::size_t>;

    auto min_position = [&](pos_type substitute, pos_type insert,
                            pos_type remove) -> pos_type {
        auto min = substitute;
        if (matrix[insert.first][insert.second] < matrix[min.first][min.second])
            min = insert;
        if (matrix[remove.first][remove.second] < matrix[min.first][min.second])
            min = remove;
        return min;
    };

    std::vector<edit> edits;
    auto r = rows - 1;
    auto c = cols - 1;
    while (r > 0 && c > 0) {
        auto min = min_position({r - 1, c - 1}, {r - 1, c}, {r, c - 1});
        if (min == pos_type{r - 1, c - 1}) {
            if (matrix[min.first][min.second] == matrix[r][c])
                edits.push_back(edit::nothing);
            else
                edits.push_back(edit::substitute);
        } else if (min == pos_type{r - 1, c}) {
            edits.push_back(edit::insert);
        } else {
            edits.push_back(edit::remove);
        }

        std::tie(r, c) = min;
    }

    // r == 0
    edits.resize(edits.size() + c, edit::remove);

    // c == 0
    edits.resize(edits.size() + r, edit::insert);

    std::reverse(std::begin(edits), std::end(edits));
    return edits;
}


template <typename RandomIt1, typename RandomIt2, typename EqualComparator>
auto levenshtein_distance_impl(RandomIt1 src_first, RandomIt1 src_last,
                               RandomIt2 dst_first, RandomIt2 dst_last,
                               EqualComparator ecomp) {
    auto src_size = src_last - src_first;
    auto dst_size = dst_last - dst_first;
    auto matrix   = build_matrix<std::size_t>(dst_size + 1, src_size + 1, 0);

    for (auto c = 0; c < src_size + 1; ++c)
        matrix[0][c] = c;
    for (auto r = 0; r < dst_size + 1; ++r)
        matrix[r][0] = r;

    for (auto r = 1; r < dst_size + 1; ++r) {
        for (auto c = 1; c < src_size + 1; ++c) {
            auto src_index = c - 1;
            auto dst_index = r - 1;
            auto cost = ecomp(src_first[src_index], dst_first[dst_index]);
            matrix[r][c] =
                std::min({matrix[r - 1][c - 1] + cost, matrix[r][c - 1] + 1,
                          matrix[r - 1][c] + 1});
        }
    }

    return matrix;
}


// return edits path
template <typename RandomIt1, typename RandomIt2, typename EqualComparator>
auto levenshtein_distance_path(RandomIt1 src_first, RandomIt1 src_last,
                               RandomIt2 dst_first, RandomIt2 dst_last,
                               EqualComparator ecomp) -> std::vector<edit> {
    auto matrix = levenshtein_distance_impl(src_first, src_last, dst_first,
                                            dst_last, ecomp);

    auto src_size = src_last - src_first;
    auto dst_size = dst_last - dst_first;
    return levenshtein_distance_edit_path(matrix, dst_size + 1, src_size + 1);
}

template <typename RandomIt1, typename RandomIt2>
auto levenshtein_distance_path(RandomIt1 src_first, RandomIt1 src_last,
                               RandomIt2 dst_first, RandomIt2 dst_last)
    -> std::vector<edit> {
    auto ecomp = [](auto&& lhs, auto&& rhs) -> std::size_t {
        return (lhs == rhs) ? 0 : 1;
    };
    return levenshtein_distance_path(src_first, src_last, dst_first, dst_last,
                                     ecomp);
}


// return number of edits
template <typename RandomIt1, typename RandomIt2, typename EqualComparator>
auto levenshtein_distance(RandomIt1 src_first, RandomIt1 src_last,
                          RandomIt2 dst_first, RandomIt2 dst_last,
                          EqualComparator ecomp) -> std::size_t {
    auto src_size = src_last - src_first;
    auto dst_size = dst_last - dst_first;
    return levenshtein_distance_impl(src_first, src_last, dst_first, dst_last,
                                     ecomp)[dst_size][src_size];
}

template <typename RandomIt1, typename RandomIt2>
auto levenshtein_distance(RandomIt1 src_first, RandomIt1 src_last,
                          RandomIt2 dst_first, RandomIt2 dst_last)
    -> std::size_t {
    auto ecomp = [](auto&& lhs, auto&& rhs) -> std::size_t {
        return (lhs == rhs) ? 0 : 1;
    };
    return levenshtein_distance(src_first, src_last, dst_first, dst_last,
                                ecomp);
}


int main() {
    using std::cbegin;
    using std::cend;

    std::string str_src{"so61pi"};
    std::string str_dst{"Huynh Ngoc Thi"};
    std::cout << "Number of edits: "
              << levenshtein_distance(cbegin(str_src), cend(str_src),
                                      cbegin(str_dst), cend(str_dst))
              << '\n';

    auto path = levenshtein_distance_path(cbegin(str_src), cend(str_src),
                                          cbegin(str_dst), cend(str_dst));
    std::cout << "Number of edits: "
              << std::count_if(cbegin(path), cend(path), [](edit val) {
                     return val != edit::nothing;
                 }) << '\n';

    auto str_transform = str_src;
    int i = 0;
    for (auto const& e : path) {
        switch (e) {
        case edit::insert:
            str_transform.insert(i, 1, str_dst[i]);
            ++i;
            break;

        case edit::remove:
            str_transform.erase(i, 1);
            break;

        case edit::substitute:
            str_transform[i] = str_dst[i];
            ++i;
            break;

        case edit::nothing:
            ++i;
            break;
        }

        std::cout << str_transform << "\n";
    }

    std::vector<int> dt_src{10, 11};
    std::vector<int> dt_dst{1, 2, 3, 4, 5};
    std::cout << "Number of edits: "
              << levenshtein_distance(cbegin(dt_src), cend(dt_src),
                                      cbegin(dt_dst), cend(dt_dst))
              << '\n';
}
