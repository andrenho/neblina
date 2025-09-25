#ifndef CONFIG_H_
#define CONFIG_H_

#include <stdbool.h>
#include <stddef.h>

#define MAX_SERVICES 64

typedef struct {
    char name[25];
    bool active;
    int  port;
    bool open_to_world;
    int  logging_color;
} ConfigService;

typedef struct {
    ConfigService services[MAX_SERVICES];
    int           services_sz;
} Config;

bool config_load(const char* json, Config* config);

#endif
