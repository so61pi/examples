#include <cerrno>
#include <cstring>
#include <iostream>

#include <boost/scope_exit.hpp>

#include <glob.h>


int globerr(char const* path, int e) {
    std::cerr << path << " " << std::strerror(e) << "\n";
	return 0;
}


int main() {
    glob_t results = {};
    auto status = glob("*.*", 0, globerr, &results);
    if (status == 0) {
        BOOST_SCOPE_EXIT_ALL(&) { globfree(&results); };
    	for (auto i = 0U; i < results.gl_pathc; i++) {
    		std::cout << results.gl_pathv[i] << "\n";
		}
		return 0;
    } else {
        switch (status) {
        case GLOB_NOSPACE:
            std::cerr << "Error: Out of memory.\n";
            break;

        case GLOB_ABORTED:
            std::cerr << "Error: Read error.\n";
            break;

        case GLOB_NOMATCH:
            std::cerr << "Error: No matches found.\n";
            break;
        }
        return 1;
    }
}
