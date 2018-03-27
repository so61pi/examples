#include <iostream>
#include <fnmatch.h>


int main() {
    auto const result = fnmatch("*.jpg", "test.jpg", 0);
    if (result == 0) {
        std::cout << "match" << std::endl;
    } else if (result == FNM_NOMATCH) {
        std::cout << "not match" << std::endl;
    } else {
        std::cout << "error" << std::endl;
    }
}
