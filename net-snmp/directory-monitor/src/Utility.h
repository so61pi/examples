#ifndef UTILITY_H
#define UTILITY_H


#include <dirent.h>

typedef void (*WALKFUNC)(struct dirent*, int, void*);

int walk_directory(char const* directory, WALKFUNC func, void* data);
int get_num_of_entries(char const* directory, unsigned char type);
int get_num_of_files(char const* directory);
int get_num_of_directories(char const* directory);


#endif /* UTILITY_H */