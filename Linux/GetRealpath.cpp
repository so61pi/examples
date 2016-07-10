#include <climits>
#include <cstdlib>
#include <iostream>
#include <string.h>

#include <boost/scope_exit.hpp>


int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Please provide a path.\n";
        return EXIT_FAILURE;
    }

    auto realPath = realpath(argv[1], nullptr);
    if (realPath) {
        BOOST_SCOPE_EXIT_ALL(&) { free(realPath); };
        std::cout << realPath << std::endl;
        return EXIT_SUCCESS;
    } else {
        char errStr[100] = {};
        strerror_r(errno, errStr, sizeof(errStr));
        std::cout << errStr << std::endl;
        return EXIT_FAILURE;
    }
}
