#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <exception>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#include <boost/scope_exit.hpp>

#include <dirent.h>


auto ReadCmdLine(std::string const& pid) -> std::string {
    auto         cmdlineFile = "/proc/" + pid + "/cmdline";
    std::fstream file{cmdlineFile};
    if (!file) throw std::runtime_error{"Cannot open file " + cmdlineFile};

    std::string cmd;
    std::getline(file, cmd);

    // linux command line args in cmdline file are separated by \0
    std::replace_if(cmd.begin(), cmd.end(),
                    [](char const& val) { return val == '\0'; }, ' ');

    return cmd;
}


int main() {
    try {
        auto proc = opendir("/proc");
        if (!proc) {
            std::cerr << "Cannot open /proc.\n";
            return EXIT_FAILURE;
        }

        BOOST_SCOPE_EXIT_ALL(&) { closedir(proc); };

        std::cout << "       PID | Command line\n"
                  << "-----------+-------------\n";

        dirent* dp = nullptr;
        while ((dp = readdir(proc)) != nullptr) {
            if (strspn(dp->d_name, "0123456789") != strlen(dp->d_name)) {
                // d_name contains something else than numbers
                continue;
            }

            std::cout << std::right << std::setw(10) << dp->d_name << " | "
                      << std::left << ReadCmdLine(dp->d_name) << std::endl;
        }
    } catch (std::exception const& e) {
        std::cerr << e.what() << std::endl;
    }
}
