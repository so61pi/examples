#include <algorithm>
#include <array>
#include <iostream>
#include <tuple>
#include <vector>


/*
The backtrack function below is similar to this function,
excepts it doesn't use recursion.

void backtrack(current, functions_set) {
    if (functions_set.is_solution(current)) {
        functions_set.proccess_solution(current);
    }
    else {
        auto its = functions_set.build(current);
        for (auto it = std::get<0>(its), eit = std::get<1>(its); it != eit; ++it) {
            functions_set.enter(*it);
            backtrack(*it, functions_set);
            functions_set.exit(*it);
        }
    }
}
*/
template<typename ForwardIt, typename FunctionsSet>
void backtrack(FunctionsSet&& funs) {
    enum {
        current_iterator,
        end_iterator,
        is_first_time
    };

    std::vector<std::tuple<ForwardIt, ForwardIt, bool>> stack;
    stack.push_back(std::tuple_cat(funs.first_build(), std::make_tuple(true)));

    while (!stack.empty()) {
        auto& back = stack.back();
        auto& it = std::get<current_iterator>(back);
        auto& eit = std::get<end_iterator>(back);

        if (std::get<is_first_time>(back)) {
            std::get<is_first_time>(back) = false;
        }
        else {
            funs.exit(*it);
            ++it;
        }

        if (it != eit) {
            funs.enter(*it);
            if (funs.is_solution(*it)) {
                funs.process_solution(*it);
            }
            else {
                stack.push_back(std::tuple_cat(funs.build(*it), std::make_tuple(true)));
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

    auto operator!=(iterator const&) const -> bool {
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


class functions_set {
public:
    functions_set(std::array<int, 6>& array, int& counter, int& solution_counter) :
        m_array{ array },
        m_counter{ counter },
        m_solution_counter{ solution_counter }
    {}


    std::pair<iterator, iterator> first_build() {
        return std::make_pair(iterator{ &m_array, 0 }, iterator{});
    }


    auto build(working_object const& wo) {
        if (wo.pos >= 6) {
            return std::make_pair(iterator{}, iterator{});
        }
        else {
            return std::make_pair(iterator{ wo.array, wo.pos + 1 }, iterator{});
        }
    }


    void enter(working_object const& wo) {
        ++m_counter;
    }


    void exit(working_object const& wo) {
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
        ++m_solution_counter;
        for (auto& e : *wo.array) {
            std::cout << e << ' ';
        }
        std::cout << "\n";
    }


private:
    std::array<int, 6>& m_array;
    int& m_counter;
    int& m_solution_counter;
};


int main() {
    std::array<int, 6> temp{};
    int counter{ 0 };
    int solution_counter{ 0 };
    functions_set funs{ temp, counter, solution_counter };
    backtrack<iterator>(funs);
    std::cout << counter << ' ' << solution_counter;
}
