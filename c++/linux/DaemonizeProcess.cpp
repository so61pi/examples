#include <cstdio>
#include <unistd.h>


int main() {
    auto stt = daemon(1, /* not change dir to / */
                      0  /* redirect all standard ios to /dev/null */);
    if (stt < 0 ) {
        std::perror("cannot daemonize process");
    }
}
