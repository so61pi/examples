#include <cstdio>
#include <cstring>
#include <iostream>

#include <unistd.h>


int main() {
    int fd[2];
    if (pipe(fd) < 0) {
        std::perror(nullptr);
    }

    int rfd = fd[0];
    int wfd = fd[1];

    char const* const data = "Hello World!";
    write(wfd, data, std::strlen(data));

    char tmp[100] = {};
    read(rfd, tmp, sizeof(tmp));

    std::cout << tmp << std::endl;

    close(rfd);
    close(wfd);
}
