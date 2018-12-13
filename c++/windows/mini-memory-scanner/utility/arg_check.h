#ifndef ARG_CHECK_H
#define ARG_CHECK_H


#include <cassert>
#include <stdexcept>


#define arg_check(condition, message)           \
    if (!(condition)) {                         \
        assert(false && message);               \
        throw std::invalid_argument(message);   \
    }


#endif // ARG_CHECK_H
