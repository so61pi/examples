#ifndef LIBCLICXX_UTILITY_H_INCLUDED
#define LIBCLICXX_UTILITY_H_INCLUDED

#include <algorithm>
#include <iterator>
#include <string>

#include <clicxx/StdEx.h>

namespace cli {
namespace util {

using stdex::string_view;

inline string_view
to_string_view(std::string const& s)
{
  return string_view(s.data(), s.size());
}

inline string_view
string_prefix(string_view a, stdex::string_view b)
{
  auto const pair = std::mismatch(a.begin(), a.end(), b.begin(), b.end());
  auto const size =
    string_view::size_type(std::distance(a.begin(), pair.first));
  return string_view(size ? a.begin() : nullptr, size);
}

template <typename It>
inline string_view
string_prefix(It first, It last)
{
  string_view result;

  for (; first != last; ++first) {
    string_view s = *first;
    if (s.empty()) {
      return string_view();
    }

    if (result.empty()) {
      result = s;
    } else {
      result = string_prefix(result, s);
      if (result.empty()) {
        return string_view();
      }
    }
  }

  return result;
}

template <typename It1, typename It2>
inline bool
start_with(It1 haystack_f, It1 haystack_l, It2 needle_f, It2 needle_l)
{
  auto const pair = std::mismatch(haystack_f, haystack_l, needle_f, needle_l);
  return pair.second == needle_l;
}

template <typename T, typename U>
inline bool
start_with(T const& haystack, U const& needle)
{
  return start_with(haystack.begin(), haystack.end(), needle.begin(),
                    needle.end());
}

} // namespace util
} // namespace cli

#endif // LIBCLICXX_UTILITY_H_INCLUDED
