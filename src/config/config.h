#ifndef CONFIG_H
#define CONFIG_H

#include <stdbool.h>
#include <stddef.h>

typedef struct {
    char* name;
    bool  active;
    int   port;
    bool  open_to_world;
    int   logging_color;
} Config_Service;

typedef struct {
    Config_Service* services;
    size_t          services_sz;
} Config;

Config* config_json_from_string(const char* json);
Config* config_json_load(const char* path);
void    config_json_free(Config* config);

#endif //CONFIG_H
