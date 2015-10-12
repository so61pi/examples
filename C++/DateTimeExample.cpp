#include <algorithm>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <string>
#include <utility>
#include <vector>
#include <boost/date_time.hpp>


namespace gregorian = boost::gregorian;


template<typename It, typename BinaryPred>
auto group_range(It first, It last, BinaryPred pred) -> std::vector<std::pair<It, It>> {
    std::vector<std::pair<It, It>> result;

    auto not2 = [pred](auto const& a, auto const& b) -> bool {
        return !pred(a, b);
    };

    while (first != last) {
        auto found = std::adjacent_find(first, last, not2);
        if (found != last) {
            std::advance(found, 1);
        }
        result.emplace_back(first, found);
        first = found;
    }

    return result;
}


template<typename It, typename UnaryPred>
auto group_point(It first, It last, UnaryPred pred) -> std::vector<std::pair<It, It>> {
    std::vector<std::pair<It, It>> result;

    while (first != last) {
        auto found = std::find_if(first, last, pred);
        if (found != last) {
            std::advance(found, 1);
        }
        result.emplace_back(first, found);
        first = found;
    }

    return result;
}


template<typename It>
auto first_date_iter(std::pair<It, It>& pair) -> It& {
    return pair.first;
}

template<typename It>
auto first_date_iter(std::pair<It, It> const& pair) -> It const& {
    return pair.first;
}

template<typename It>
auto last_date_iter(std::pair<It, It>& pair) -> It& {
    return pair.second;
}

template<typename It>
auto last_date_iter(std::pair<It, It> const& pair) -> It const& {
    return pair.second;
}


template<typename It>
auto group_by_week(It first_date, It last_date) {
    return group_point(first_date, last_date,
        [](auto const& date) { return date.day_of_week() == gregorian::Saturday; });
}


template<typename It>
auto group_by_moth(It first_date, It last_date) {
    return group_range(first_date, last_date,
        [](auto const& a, auto const& b) { return a.month() == b.month(); });
}


auto create_dates(gregorian::day_iterator first_date, gregorian::date last_date) -> std::vector<gregorian::date> {
    std::vector<gregorian::date> dates;
    while (first_date != last_date) {
        dates.emplace_back(*first_date);
        ++first_date;
    }
    return dates;
}


template<typename It>
auto group_dates(It first, It last) {
    auto months = group_by_moth(first, last);
    std::vector<decltype(group_by_week(first_date_iter(months[0]), last_date_iter(months[0])))> weeks_in_months;
    for (auto const& month : months) {
        weeks_in_months.emplace_back(group_by_week(first_date_iter(month), last_date_iter(month)));
    }
    return weeks_in_months;
}


void print_week_title(size_t width) {
    char const *titles[] = { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };
    for (auto const& title : titles) {
        std::cout << std::setw(width) << title;
    }
}


template<typename It>
void print_week_dates(It first_date, It last_date, size_t width) {
    std::cout << std::string(first_date->day_of_week() * width, ' ');
    while (first_date != last_date) {
        std::cout << std::setw(width) << first_date->day();
        ++first_date;
    }
}


template<typename T>
void print_calendar(T const& weeks_in_months) {
    std::size_t const width = 5;
    for (auto const& month : weeks_in_months) {
        print_week_title(width);
        std::cout << '\n';
        for (auto const& week : month) {
            print_week_dates(first_date_iter(week), last_date_iter(week), width);
            std::cout << '\n';
        }
        std::cout << "\n\n";
    }
}


void print_year(int year) {
    gregorian::day_iterator it{ gregorian::date{ year, 1, 1 } };
    gregorian::date end{ year + 1, 1, 1 };

    auto dates = create_dates(it, end);
    auto weeks_in_months = group_dates(std::begin(dates), std::end(dates));
    print_calendar(weeks_in_months);
}


int main() {
    print_year(2015);
}
