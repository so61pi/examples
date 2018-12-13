#include <cstdio>
#include <exception>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#include <sys/time.h>
#include <sys/resource.h>


int main() {
    /// RLIMIT_DATA maybe have no effect on malloc which can be implemented by using mmap.
    /// Should read more about OOM killer.
    rlimit rlim;
    if (getrlimit(RLIMIT_AS, &rlim) < 0) {
        return 1;
    }

    rlim.rlim_cur = rlim.rlim_max = 20*1024*1024 - 1;
    if (setrlimit(RLIMIT_AS, &rlim) < 0) {
        return 1;
    }

    try {
        std::vector<std::string> v;
        while (true) {
            v.push_back("so61pi");
        }
        return 0;
    } catch (std::bad_alloc const& e) {
        puts("bad_alloc");
    } catch (std::exception const& e) {
        puts("exception");
    } catch (...) {
        puts("unknown");
    }
    return 1;
}
