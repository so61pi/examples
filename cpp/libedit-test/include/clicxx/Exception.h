#ifndef LIBCLICXX_EXCEPTION_H_INCLUDED
#define LIBCLICXX_EXCEPTION_H_INCLUDED

#include <exception>
#include <stdexcept>

namespace cli {
namespace exception {

class Runtime : public std::runtime_error
{
  using std::runtime_error::runtime_error;
};

class Logical : public std::logic_error
{
  using std::logic_error::logic_error;
};

} // namespace exception
} // namespace cli

#endif // LIBCLICXX_EXCEPTION_H_INCLUDED
