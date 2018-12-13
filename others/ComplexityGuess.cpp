#include <algorithm>
#include <cassert>
#include <chrono>
#include <cinttypes>
#include <cmath>
#include <cstddef>
#include <exception>
#include <functional>
#include <iostream>
#include <iterator>
#include <list>
#include <numeric>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>


class complexity {
public:
    enum class category {
        O_1,
        O_logN,
        O_N,
        O_NlogN,
        O_N2
    };


    static auto name(category cat) -> char const * {
        switch (cat) {
        case complexity::category::O_1:
            return "O(1)";

        case complexity::category::O_logN:
            return "O(logN)";

        case complexity::category::O_N:
            return "O(N)";

        case complexity::category::O_NlogN:
            return "O(NlogN)";

        case complexity::category::O_N2:
            return "O(N2)";

        default:
            throw std::invalid_argument{ "invalid complexity category" };
        }
    }


    static auto details(std::uintmax_t t1, std::uintmax_t t2, std::uintmax_t n1, std::uintmax_t n2) -> std::string {
        auto t2t1_ratio = static_cast<double>(t2) / t1;

        std::vector<std::pair<category, double>> cat;
        cat.emplace_back(category::O_1,         1);
        cat.emplace_back(category::O_logN,      std::log2(n2) / std::log2(n1));
        cat.emplace_back(category::O_N,         static_cast<double>(n2) / n1);
        cat.emplace_back(category::O_NlogN,     n2 / n1 * std::log2(n2) / std::log2(n1));
        cat.emplace_back(category::O_N2,        static_cast<double>(n2 * n2) / (n1 * n1));

        std::stringstream ss;
        ss  << "T1 = " << t1 << "\n"
            << "T2 = " << t2 << "\n"
            << "N1 = " << n1 << "\n"
            << "N2 = " << n2 << "\n\n"
            << "T2\n"
            << "-- = " << t2t1_ratio << "\n"
            << "T1\n\n";
        for (auto& e : cat) {
            std::string temp{ name(e.first) };
            temp.append(temp.size() < 16 ? 16 - temp.size() : 0, ' ');
            ss << temp << " = " << e.second << "\n";
        }

        return ss.str();
    };


    static auto guess(std::uintmax_t t1, std::uintmax_t t2, std::uintmax_t n1, std::uintmax_t n2) -> category {
        auto t2t1_ratio = static_cast<double>(t2) / t1;

        std::vector<std::pair<category, double>> cat;
        cat.emplace_back(category::O_1,         1);
        cat.emplace_back(category::O_logN,      std::log2(n2) / std::log2(n1));
        cat.emplace_back(category::O_N,         static_cast<double>(n2) / n1);
        cat.emplace_back(category::O_NlogN,     n2 / n1 * std::log2(n2) / std::log2(n1));
        cat.emplace_back(category::O_N2,        static_cast<double>(n2 * n2) / (n1 * n1));

        std::vector<double> temp;
        for (auto& e : cat) {
            temp.push_back(std::abs(t2t1_ratio / e.second - 1));
        }

        auto pos = std::min_element(begin(temp), end(temp));
        assert(pos != end(temp));
        return cat[std::distance(begin(temp), pos)].first;
    };
    

    static auto help() -> std::string {
        return
            "O(1)                               \n"
            "    T2                             \n"
            "    -- = 1                         \n"
            "    T1                             \n\n"
            "O(logN)                            \n"
            "    T2   O(logN2)   logN2          \n"
            "    -- = -------- = -----          \n"
            "    T1   O(logN1)   logN1          \n\n"
            "O(N)                               \n"
            "    T2   O(N2)   N2                \n"
            "    -- = ----- = --                \n"
            "    T1   O(N1)   N1                \n\n"
            "O(NlogN)                           \n"
            "    T2   O(N2logN2)   N2logN2      \n"
            "    -- = ---------- = -------      \n"
            "    T1   O(N1logN1)   N1logN1      \n\n"
            "O(N^2)                             \n"
            "    T2   O(N2^2)   N2^2            \n"
            "    -- = ------- = ----            \n"
            "    T1   O(N1^2)   N1^2            \n\n"
            ;
    };
};


int main() {
    auto n1 = 1000000;
    std::vector<int> a;
    a.resize(n1);
    std::iota(begin(a), end(a), 0);

    auto n2 = 10000000;
    std::vector<int> b;
    b.resize(n2);
    std::iota(begin(b), end(b), 0);

    int v1{ 0 };
    std::chrono::high_resolution_clock::duration t1{};
    {
        auto start = std::chrono::high_resolution_clock::now();
        std::sort(begin(a), end(a), std::greater<void>{});
        v1 = a.front();
        auto end = std::chrono::high_resolution_clock::now();
        t1 = end - start;
    }

    int v2{ 0 };
    std::chrono::high_resolution_clock::duration t2{};
    {
        auto start = std::chrono::high_resolution_clock::now();
        std::sort(begin(b), end(b), std::greater<void>{});
        v2 = b.front();
        auto end = std::chrono::high_resolution_clock::now();
        t2 = end - start;
    }

	std::cout << v1 << v2 << "\n\n";
    std::cout << "complexity : " << complexity::name(complexity::guess(t1.count(), t2.count(), n1, n2)) << "\n\n";
    std::cout << "[details]\n" << complexity::details(t1.count(), t2.count(), n1, n2) << "\n\n";
}
