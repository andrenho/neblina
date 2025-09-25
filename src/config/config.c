#include "config.h"

#include <string.h>

#include "microjson/mjson.h"
#include "common.h"

bool config_load(const char* json, Config* config)
{
    const struct json_attr_t service_attrs[] = {
        { "name",         t_string,  STRUCTOBJECT(ConfigService, name), .len = sizeof(config->services[0].name) },
        { "active",       t_boolean, STRUCTOBJECT(ConfigService, active) },
        { "port",         t_integer, STRUCTOBJECT(ConfigService, port) },
        { "openToWorld",  t_boolean, STRUCTOBJECT(ConfigService, open_to_world) },
        { "loggingColor", t_integer, STRUCTOBJECT(ConfigService, logging_color) },
        { "",       t_ignore,  .addr = NULL },
        { NULL },
    };

    const struct json_attr_t config_attrs[] = {
        { "services", t_array, STRUCTARRAY(config->services, service_attrs, &config->services_sz) },
        { "",         t_ignore, .addr = NULL },
        { NULL },
    };

    memset(config->services, 0, MAX_SERVICES * sizeof(ConfigService));

    int status = json_read_object(json, config_attrs, NULL);
    if (status != 0)
        THROW("%s", json_error_string(status));
    return true;
}