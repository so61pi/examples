#include "GlobalSettings.h"


/*
 * return modifiable settings
 */
global_settings_t* get_mutable_global_settings() {
    static global_settings_t settings;
    return &settings;
}


/*
 * return unmodifiable settings
 */
global_settings_t const* get_global_settings() {
    return get_mutable_global_settings();
}
