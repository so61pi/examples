#include <iostream>

#include <boost/scope_exit.hpp>
#include <zlib.h>

/***************************************
mkdir build
cd build
cmake ..
cmake --build .
****************************************/

int main() {
    std::cout << zlibVersion() << std::endl;
}
