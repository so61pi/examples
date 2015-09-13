#include <algorithm>
#include <array>
#include <cassert>
#include <iostream>
#include <tuple>
#include <vector>


/*
The backtrack function below is similar to this function,
excepts it doesn't use recursion.

void backtrack(current, visitor) {
    if (visitor.is_solution(current)) {
        visitor.proccess_solution(visitor);
        return;
    }
    else {
        auto its = visitor.construct_solution(current);
        for (auto it = std::get<0>(its), eit = std::get<1>(its); it != eit; ++it) {
            visitor.pre_backtrack(*it);
            backtrack(*it, visitor);
            visitor.post_backtrack(*it);
        }
    }
}
*/
template<typename ForwardIt, typename Visitor>
void backtrack(Visitor&& visitor) {
    enum {
        current_iterator,
        end_iterator,
        is_first_time
    };

    std::vector<std::tuple<ForwardIt, ForwardIt, bool>> stack;
    stack.push_back(std::tuple_cat(visitor.first_construct(), std::make_tuple(true)));

    while (!stack.empty()) {
        auto& back = stack.back();
        auto& it = std::get<current_iterator>(back);
        auto& eit = std::get<end_iterator>(back);

        if (std::get<is_first_time>(back)) {
            std::get<is_first_time>(back) = false;
        }
        else {
            visitor.post_backtrack(*it);
            ++it;
        }

        if (it != eit) {
            visitor.pre_backtrack(*it);
            if (visitor.is_solution(*it)) {
                visitor.process_solution(*it);
            }
            else {
                stack.push_back(std::tuple_cat(visitor.construct_solution(*it), std::make_tuple(true)));
            }
        }
        else {
            stack.pop_back();
        }
    }
}


struct working_object {
    working_object() = default;
    working_object(std::array<int, 6> *a, int p) :
        array{ a },
        pos{ p }
    {}

    std::array<int, 6> *array{ nullptr };
    int pos{ 0 };
};


struct iterator {
    iterator() = default;
    iterator(std::array<int, 6> *array, int pos) :
        m_wo{ array, pos },
        m_is_end{ false }
    {
        if (m_wo.pos >= 6) {
            m_is_end = true;
        }
    }

    auto operator!=(iterator const& other) const -> bool {
        return !m_is_end;
    }

    auto operator++() -> iterator& {
        ++m_current_index;
        if (m_current_index >= 6) {
            m_is_end = true;
        }
        return *this;
    }

    auto operator*() const -> working_object const& {
        (*m_wo.array)[m_wo.pos] = m_set[m_current_index];
        return m_wo;
    }


private:
    working_object m_wo;
    std::vector<int> m_set{ 1, 2, 3, 4, 5, 6 };
    int m_current_index{ 0 };
    bool m_is_end{ true };
};


class visitor {
public:
    visitor(std::array<int, 6>& array, int& counter) :
        m_array{ array },
        m_counter{ counter }
    {}


    std::pair<iterator, iterator> first_construct() {
        return std::make_pair(iterator{ &m_array, 0 }, iterator{});
    }


    auto construct_solution(working_object wo) {
        if (wo.pos >= 6) {
            return std::make_pair(iterator{}, iterator{});
        }
        else {
            return std::make_pair(iterator{ wo.array, wo.pos + 1 }, iterator{});
        }
    }


    void pre_backtrack(working_object const&) {
    }


    void post_backtrack(working_object const&) {
    }


    bool is_solution(working_object const& wo) {
        auto temp = *wo.array;
        std::sort(begin(temp), end(temp));

        for (auto i = 0; i < 6; ++i) {
            if (i + 1 != temp[i]) {
                return false;
            }
        }

        return true;
    }


    void process_solution(working_object const& wo) {
        ++m_counter;
        for (auto& e : *wo.array) {
            std::cout << e << ' ';
        }
        std::cout << "\n";
    }


private:
    std::array<int, 6>& m_array;
    int& m_counter;
};


int main() {
    std::array<int, 6> temp;
    int counter{ 0 };
    visitor vis{ temp, counter };
    backtrack<iterator>(vis);
    std::cout << counter;
}
