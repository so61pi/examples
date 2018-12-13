#include <iostream>

#include <boost/scope_exit.hpp>

#include <wordexp.h>


int main() {
    wordexp_t result = {};
    auto status = wordexp("~", &result, WRDE_NOCMD);
    if (status == 0) {
        BOOST_SCOPE_EXIT_ALL(&) { wordfree(&result); };
        for (auto i = 0U; i < result.we_wordc; ++i) {
            std::cout << result.we_wordv[i] << "\n";
        }
        return 0;
    } else {
        switch (status) {
        case WRDE_BADCHAR:
            std::cerr << "Error: Invalid character in pattern.\n";
            break;

        case WRDE_BADVAL:
            std::cerr << "Error: Undefined shell variable.\n";
            break;

        case WRDE_CMDSUB:
            std::cerr << "Error: Command substitution failed.\n";
            break;

        case WRDE_NOSPACE:
            std::cerr << "Error: Out of memory.\n";
            break;

        case WRDE_SYNTAX:
            std::cerr << "Error: Shell syntax error.\n";
            break;

        default:
            std::cerr << "Error: Unknown error.\n";
        }
        return 1;
    }
}
