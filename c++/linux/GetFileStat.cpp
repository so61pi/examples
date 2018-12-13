#include <cstdlib>
#include <cstring>
#include <ctime>
#include <iostream>
#include <string>

#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>


std::string mode_to_string(mode_t mode) {
    std::string strType;
#define APPENDTYPE(TYPE) if (TYPE(mode)) strType += (strType.empty() ? #TYPE : " & "#TYPE)
    APPENDTYPE(S_ISREG);
    APPENDTYPE(S_ISDIR);
    APPENDTYPE(S_ISCHR);
    APPENDTYPE(S_ISBLK);
    APPENDTYPE(S_ISFIFO);
    APPENDTYPE(S_ISLNK);
    APPENDTYPE(S_ISSOCK);
#undef APPENDTYPE

    std::string strMode;
#define APPENDMODE(MODE) if (mode & MODE) strMode += (strMode.empty() ? #MODE : " | "#MODE)
    // owner
    APPENDMODE(S_IRUSR);
    APPENDMODE(S_IWUSR);
    APPENDMODE(S_IXUSR);

    // group
    APPENDMODE(S_IRGRP);
    APPENDMODE(S_IWGRP);
    APPENDMODE(S_IXGRP);

    // other
    APPENDMODE(S_IROTH);
    APPENDMODE(S_IWOTH);
    APPENDMODE(S_IXOTH);
#undef APPENDMODE

    return strType + " --- " + strMode;
}


std::string time_to_string(time_t time) {
    tm tm = {};
    localtime_r(&time, &tm);
    char strTime[100] = {};
    std::strftime(strTime, sizeof(strTime) - 1, "%Y %m %d - %H:%M:%S", &tm);
    return strTime;
}


std::string to_string(struct stat const& stat, std::size_t indent = 4) {
    using std::to_string;

    std::string result;
    result += std::string(indent, ' ') + "st_dev     : " + to_string(stat.st_dev) + "\n"
            + std::string(indent, ' ') + "st_ino     : " + to_string(stat.st_ino) + "\n"
            + std::string(indent, ' ') + "st_mode    : " + mode_to_string(stat.st_mode) + "\n"
            + std::string(indent, ' ') + "st_nlink   : " + to_string(stat.st_nlink) + "\n"
            + std::string(indent, ' ') + "st_uid     : " + to_string(stat.st_uid) + "\n"
            + std::string(indent, ' ') + "st_gid     : " + to_string(stat.st_gid) + "\n"
            + std::string(indent, ' ') + "st_rdev    : " + to_string(stat.st_rdev) + "\n"
            + std::string(indent, ' ') + "st_size    : " + to_string(stat.st_size) + "\n"
            + std::string(indent, ' ') + "st_blksize : " + to_string(stat.st_blksize) + "\n"
            + std::string(indent, ' ') + "st_blocks  : " + to_string(stat.st_blocks) + "\n"
            + std::string(indent, ' ') + "st_atime   : " + time_to_string(stat.st_atime) + "\n"
            + std::string(indent, ' ') + "st_mtime   : " + time_to_string(stat.st_mtime) + "\n"
            + std::string(indent, ' ') + "st_ctime   : " + time_to_string(stat.st_ctime) + "\n";

    return result;
}


int main() {
    auto file = "/usr/bin/java";

    struct stat fileStat;
    if (lstat(file, &fileStat) == -1) {
        auto errnum = errno;
        std::cerr << "lstat error : " << std::strerror(errnum) << std::endl;
        return EXIT_FAILURE;
    } else {
        std::cout << "lstat result of " << file << "\n"
                  << to_string(fileStat) << std::flush;
    }

    std::cout << std::endl;
    if (stat(file, &fileStat) == -1) {
        auto errnum = errno;
        std::cerr << "stat error : " << std::strerror(errnum) << std::endl;
        return EXIT_FAILURE;
    } else {
        std::cout << "stat result of " << file << "\n"
                  << to_string(fileStat) << std::flush;
    }

    return EXIT_SUCCESS;
}
