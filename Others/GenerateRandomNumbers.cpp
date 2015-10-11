#include <algorithm>
#include <forward_list>
#include <iostream>
#include <iterator>
#include <list>
#include <random>
#include <vector>


template<
    typename Engine,
    typename Distributor,
    typename ForwardIt
>
void generate_random(
    ForwardIt first,
    ForwardIt last,
    Engine& e,
    Distributor& d
)
{
    std::generate(first, last, [&] { return d(e); });
}


template<
    typename Engine,
    typename Distributor,
    typename ForwardIt
>
void generate_random(
    ForwardIt first,
    ForwardIt last,
    Engine& e,
    Distributor& d,
    typename Distributor::result_type a,
    typename Distributor::result_type b
)
{
    typename Distributor::param_type range{ a, b };
    std::generate(first, last, [&] { return d(e, range); });
}


template<
    typename T,
    typename ForwardIt
>
void generate_random(
    ForwardIt first,
    ForwardIt last,
    T a = 0,
    T b = std::numeric_limits<T>::max()
)
{
    std::random_device e{};
    std::uniform_int_distribution<> d{ a, b };
    generate_random(first, last, e, d);
}


int main() {
    { //
        std::forward_list<char> fl;
        fl.resize(10);
        generate_random(begin(fl), end(fl), 'a', 'z');
        for (auto& e : fl) {
            std::cout << e << ' ';
        }
        std::cout << "\n";
    }

    { //
        std::mt19937 mt{ std::random_device{}() };
        std::uniform_int_distribution<> uid;

        std::list<int> l;
        l.resize(10);
        generate_random(begin(l), end(l), mt, uid, 0, 10);
        for (auto& e : l) {
            std::cout << e << ' ';
        }
        std::cout << "\n";
    }

    { //
        std::vector<double> v;
        v.resize(10);
        std::random_device rd;
        generate_random(begin(v), end(v), rd, std::generate_canonical<double, 10, decltype(rd)>);

        for (auto& e : v) {
            std::cout << e << ' ';
        }
        std::cout << "\n";
    }
}
