#include <type_traits>


template<typename... Ts>
struct all_same;

template<typename First, typename Second, typename... Rest>
struct all_same<First, Second, Rest...> : std::integral_constant<bool, std::is_same<First, Second>::value && all_same<Second, Rest...>::value>
{};

template<typename First, typename Second>
struct all_same<First, Second> : std::integral_constant<bool, std::is_same<First, Second>::value>
{};


int main() {
    static_assert(all_same<int, int, int>::value, "error");
}
