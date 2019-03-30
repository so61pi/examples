#ifndef LIBCLICXX_CHECK_H_INCLUDED
#define LIBCLICXX_CHECK_H_INCLUDED

#include <boost/preprocessor/stringize.hpp>

#include <clicxx/Exception.h>

#define SRCLOCATION __FILE__ ":" BOOST_PP_STRINGIZE(__LINE__)

#define CLI_CHECK(COND)                                                        \
  do {                                                                         \
    if (!(COND)) {                                                             \
      throw ::cli::exception::Logical(#COND " violated @ " SRCLOCATION);       \
    }                                                                          \
  } while (0)

#define CLI_RCHECK(COND)                                                       \
  do {                                                                         \
    if (!(COND)) {                                                             \
      throw ::cli::exception::Runtime(#COND " failed @ " SRCLOCATION);         \
    }                                                                          \
  } while (0)

#define CLI_UNREACHABLE()                                                      \
  do {                                                                         \
    throw ::cli::exception::Logical("Unreachable reached @ " SRCLOCATION);     \
  } while (0)

#endif // LIBCLICXX_CHECK_H_INCLUDED
