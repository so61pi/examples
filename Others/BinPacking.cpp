#include <algorithm>
#include <cstddef>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <set>
#include <vector>


struct object {
    static constexpr std::size_t dummy = (std::size_t)-1;

    std::size_t const identity;
    int const weight;
};

auto operator<<(std::ostream& os, object const& obj) -> std::ostream& {
    os << "[" << obj.identity << "] " << obj.weight;
    return os;
}

auto operator<(object const& lhs, object const& rhs) -> bool {
    return lhs.weight < rhs.weight;
}


struct bin {
    int capacity;
    std::vector<object> objects;
};


auto bin_packing(std::set<object>& objs) -> std::vector<bin> {
    auto find_best_fit = [](auto& objs, int capacity) {
        auto it = objs.upper_bound(object{ object::dummy, capacity });
        if (it == objs.begin() || objs.empty())
            return objs.end();
        else
            return std::prev(it);
    };

    std::vector<bin> bins;
    bins.push_back(bin{ 10 });
    while (!objs.empty()) {
        auto& current_bin = bins.back();
        auto found = find_best_fit(objs, current_bin.capacity);
        if (found != objs.end()) {
            current_bin.capacity -= found->weight;
            current_bin.objects.push_back(*found);
            objs.erase(found);
        }
        else {
            // if we cannot find a good object even if nothing is in bin
            // then all left objects are too big
            if (current_bin.objects.empty()) {
                break;
            }

            bins.push_back(bin{ 10 });
        }
    }

    return bins;
}


int main() {
    std::set<object> objs{
        object{ 1, 3 }, object{ 2, 7 }, object{ 3, 5 },
        object{ 4, 4 }, object{ 5, 8 }, object{ 6, 2 }, 
        object{ 4, 11 }
    };
    auto bins = bin_packing(objs);
    for (auto const& bin : bins) {
        std::copy(std::begin(bin.objects), std::end(bin.objects),
            std::ostream_iterator<object>{ std::cout, "  " });
        std::cout << "[left capacity] " << bin.capacity;
        std::cout << '\n';
    }
}
