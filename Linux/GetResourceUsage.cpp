#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <iostream>

#include <sys/resource.h>
#include <sys/time.h>


long long delta(timeval const& x, timeval const& y) {
    return (x.tv_sec * 1000000 + x.tv_usec) - (y.tv_sec * 1000000 + y.tv_usec);
}


int main() {
    rusage begin;
    if (getrusage(RUSAGE_SELF, &begin) < 0) {
        std::cerr << std::strerror(errno) << std::endl;
        return EXIT_FAILURE;
    }

    for (volatile int i = 0; i < 1000000; ++i) {
        // nothing
    }

    rusage end;
    if (getrusage(RUSAGE_SELF, &end) < 0) {
        std::cerr << std::strerror(errno) << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "CPU in ms (user) : " << delta(end.ru_utime, begin.ru_utime) << "\n"
              << "          (sys)  : " << delta(end.ru_stime, begin.ru_stime) << "\n";
    return EXIT_SUCCESS;
}
