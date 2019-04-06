#include "Utility.h"

#include <assert.h>
#include <stddef.h>


int walk_directory(char const* directory, WALKFUNC func, void* data) {
    assert(directory && "directory must not be NULL");

    int nentries = 0;
    DIR* dir = opendir(directory);
    struct dirent* entry = NULL;
    while ((entry = readdir(dir)) != NULL) {
        func(entry, nentries, data);
        ++nentries;
    }
    closedir(dir);

    return nentries;
}


/*
 * get number of entries of paticular type in a directory
 */
int get_num_of_entries(char const* directory, unsigned char type) {
    assert(directory && "directory must not be NULL");

    int nentries = 0;
    DIR* dir = opendir(directory);
    struct dirent* entry = NULL;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == type) {
            ++nentries;
        }
    }
    closedir(dir);

    return nentries;
}


/*
 * return number of regular files in directory
 */
int get_num_of_files(char const* directory) {
    return get_num_of_entries(directory, DT_REG);
}


/*
 * return number of sub directories in directory
 */
int get_num_of_directories(char const* directory) {
    return get_num_of_entries(directory, DT_DIR);
}
