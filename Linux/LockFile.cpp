#include <sys/file.h>
#include <sys/stat.h>
#include <unistd.h>


///
/// \file
/// \brief
/// Please add your own error handling.
///


int LockFile(char const* lockfile) {
    struct stat st0;
    struct stat st1;

    int fd = -1;
    while (true) {
        fd = open(lockfile, O_CREAT);
        flock(fd, LOCK_EX);
        fstat(fd, &st0);

        stat(lockfile, &st1);
        if (st0.st_ino == st1.st_ino) break;

        close(fd);
    }

    return fd;
}


int UnlockFile(char const* lockfile, int fd) {
    unlink(lockfile);
    flock(fd, LOCK_UN);
    return 0;
}


int main() {
    auto const lockfile = "/var/lock/test.lock";
    auto const fdlock = LockFile(lockfile);

    // Do something.

    UnlockFile(lockfile, fdlock);
}
