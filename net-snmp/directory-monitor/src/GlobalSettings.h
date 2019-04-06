#ifndef GLOBALSETTINGS_H
#define GLOBALSETTINGS_H


#include <linux/limits.h>


typedef struct {
    char path[PATH_MAX + 1];
    int debug;
    int output;
} global_settings_t;


global_settings_t* get_mutable_global_settings();
global_settings_t const* get_global_settings();


#endif /* GLOBALSETTINGS_H */
