#include "config.h"

#include <string.h>

#include "microjson/mjson.h"

bool config_load(const char* json, Config* config)
{
    const struct json_attr_t service_attrs[] = {
        { "port", t_integer, STRUCTOBJECT(ConfigService, port), .len = sizeof(int) },
        { "",     t_ignore,  .addr = NULL },
        { NULL },
    };

    const struct json_attr_t config_attrs[] = {
        { "services", t_array, STRUCTARRAY(config->services, service_attrs, &config->services_sz) },
        { "",         t_ignore, .addr = NULL },
        { NULL },
    };

    memset(config->services, 0, MAX_SERVICES * sizeof(ConfigService));

    int status = json_read_object(json, config_attrs, NULL);
    printf("%s\n", json_error_string(status));
    printf("%d\n", config->services[0].port);
    // TODO - get error description
    return status == 0;
}

void config_free(Config* config)
{
}