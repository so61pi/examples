#include <cstdio>
#include <iostream>
#include <string>

#include <fcntl.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>


int main() {
    auto file = "/home/hnthi/work/tmp/mmap-example/build-mmap-example-Desktop-Default/Makefile";

    auto fd = open(file, O_RDWR);
    if (fd == -1) {
        std::perror("open");
        return EXIT_FAILURE;
    }

    struct stat stat;
    if (fstat(fd, &stat) == -1) {
        std::perror("fstat");
        return EXIT_FAILURE;
    }

    if (!S_ISREG(stat.st_mode)) {
        std::cerr << file << " is not a file" << std::endl;
        return EXIT_FAILURE;
    }

    auto p = mmap(nullptr, stat.st_size, PROT_READ /*| PROT_WRITE*/, MAP_SHARED, fd, 0);
    if (p == MAP_FAILED) {
        std::perror("mmap");
        return EXIT_FAILURE;
    }

    auto fileMem = reinterpret_cast<std::uint8_t*>(p);

    // it's safe to close the file now, mapped file is still in memory
    close(fd);

    // process the file content
    if (stat.st_size) {
        // print last 10 characters
        auto last = stat.st_size;
        auto first = (last > 10 ? last - 10 : 0);
        for (auto i = first; i < last; ++i) {
            std::cout << (fileMem[i]) << ' ';
        }
        std::cout << std::endl;
    } else {
        std::cout << "file is empty" << std::endl;
    }

    // unmap
    if (munmap(p, stat.st_size) == -1) {
        std::perror("unmap");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
