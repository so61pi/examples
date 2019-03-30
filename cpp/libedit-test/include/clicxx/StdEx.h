#ifndef LIBCLICXX_STD_EX_H_INCLUDED
#define LIBCLICXX_STD_EX_H_INCLUDED

#include <cstring>
#include <iterator>
#include <ostream>
#include <string>
#include <type_traits>

namespace cli {
namespace stdex {

class string_view
{
public:
  typedef char value_type;
  typedef char* pointer;
  typedef char const* const_pointer;
  typedef char& reference;
  typedef char const& const_reference;
  typedef char const* const_iterator;
  typedef const_iterator iterator;
  typedef std::size_t size_type;

public:
  string_view()
    : m_size(0)
    , m_string(nullptr)
  {
  }

  string_view(const_pointer s, size_type size) noexcept : m_size(size),
                                                          m_string(s)
  {
  }

  string_view(const_pointer s) noexcept : m_size(std::strlen(s)), m_string(s) {}

  const_iterator begin() const noexcept { return m_string; }
  const_iterator cbegin() const noexcept { return m_string; }

  const_iterator end() const noexcept { return m_string + m_size; }
  const_iterator cend() const noexcept { return m_string + m_size; }

  const_reference operator[](size_type pos) const { return m_string[pos]; }

  const_reference front() const { return *m_string; }
  const_reference back() const { return m_string[m_size - 1]; }
  const_pointer data() const noexcept { return m_string; }

  size_type size() const noexcept { return m_size; }
  size_type length() const noexcept { return m_size; }

  bool empty() const noexcept { return size() == 0; }

private:
  size_type m_size;
  const_pointer m_string;
};

inline bool
operator==(string_view lhs, string_view rhs)
{
  return std::equal(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

} // namespace stdex
} // namespace cli

#endif // LIBCLICXX_STD_EX_H_INCLUDED
