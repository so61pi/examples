#ifndef SYSTEM_EXCEPT_H
#define SYSTEM_EXCEPT_H


#include <string>
#include <system_error>


inline std::system_error make_system_except(int ec, char const *what) {
    return std::system_error{ ec, std::system_category(), what };
}


#endif // SYSTEM_EXCEPT_H
