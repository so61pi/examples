#include <algorithm>
#include <cstddef>
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
}


template<typename RandomIt1, typename RandomIt2, typename EqualComparator>
auto levenshtein_distance_impl(
    RandomIt1 src_first, RandomIt1 src_last,
    RandomIt2 dst_first, RandomIt2 dst_last,
    EqualComparator ecomp
)
{
    auto src_size = src_last - src_first;
    auto dst_size = dst_last - dst_first;
    auto matrix = build_matrix<std::size_t>(dst_size + 1, src_size + 1, 0);

    for (auto c = 0; c < src_size + 1; ++c)
        matrix[0][c] = c;
    for (auto r = 0; r < dst_size + 1; ++r)
        matrix[r][0] = r;

    for (auto r = 1; r < dst_size + 1; ++r) {
        for (auto c = 1; c < src_size + 1; ++c) {
            auto src_index = c - 1;
            auto dst_index = r - 1;
            auto cost = !ecomp(src_first[src_index], dst_first[dst_index]) ? 1 : 0;
            matrix[r][c] =
                std::min({ matrix[r - 1][c - 1] + cost, matrix[r][c - 1] + 1, matrix[r - 1][c] + 1 });
        }
    }

    return matrix;
}


// return number of edits
template<typename RandomIt1, typename RandomIt2, typename EqualComparator>
auto levenshtein_distance(
    RandomIt1 src_first, RandomIt1 src_last,
    RandomIt2 dst_first, RandomIt2 dst_last,
    EqualComparator ecomp
) -> std::size_t
{
    auto src_size = src_last - src_first;
    auto dst_size = dst_last - dst_first;
    return levenshtein_distance_impl(src_first, src_last, dst_first, dst_last, ecomp)[dst_size][src_size];
}

template<typename RandomIt1, typename RandomIt2>
auto levenshtein_distance(
    RandomIt1 src_first, RandomIt1 src_last,
    RandomIt2 dst_first, RandomIt2 dst_last
) -> std::size_t
{
    return levenshtein_distance(src_first, src_last, dst_first, dst_last, std::equal_to<>{});
}


struct score_record {
    int index;
    std::size_t score;
};


// create score for each word
template<typename ForwardIt, typename T>
auto create_scores(ForwardIt first, ForwardIt last,
                   T const& candidate) -> std::vector<score_record> {
    using std::begin;
    using std::end;

    std::vector<score_record> scores;
    for (auto i = 0; first != last; ++first, ++i) {
        auto score = levenshtein_distance(cbegin(*first), cend(*first),
                                          cbegin(candidate), cend(candidate));
        scores.push_back({ i, score });
    }

    return scores;
}


int main() {
    std::vector<std::string> words{ "abandon",
                                    "abandoned",
                                    "ability",
                                    "able",
                                    "about",
                                    "above",
                                    "abroad",
                                    "absence",
                                    "absent",
                                    "absolute",
                                    "absolutely",
                                    "absorb" };
    std::string const mistyped{ "sbsrnty" }; // supposed to be 'absent'
    auto scores = create_scores(words.cbegin(), words.cend(), mistyped);

    // stable sort to preserve the words order
    std::stable_sort(scores.begin(), scores.end(),
        [](score_record const& a, score_record const& b) -> bool {
            return a.score < b.score;
        });

    auto gap = std::adjacent_find(scores.cbegin(), scores.cend(),
        [](score_record const& a, score_record const& b) {
            return !(a.score == b.score);
        });
    if (gap != scores.end()) ++gap;
    
    // print all the entries with the lowest score
    std::for_each(scores.cbegin(), gap,
                  [&](score_record const& record) {
                      std::cout << words[record.index] << ' ' << record.score << '\n';
                  });
}
