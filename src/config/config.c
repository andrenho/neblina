#include "config.h"

#define JSMN_HEADER
#include <jsmn.h>

#include "common.h"
#include "file/whole_file.h"

#define MAX_TOKENS 2048

#define EQ_STR(json, tok, txt) ((tok)->type == JSMN_STRING && (int)strlen(txt) == (tok)->end - (tok)->start && strncmp((json) + (tok)->start, (txt), (tok)->end - (tok)->start)
#define VALIDATE(

Config* config_json_from_string(const char* json)
{
    Config* config = calloc(1, sizeof(Config));

    jsmn_parser p;
    jsmn_init(&p);

    jsmntok_t t[MAX_TOKENS];
    int r = jsmn_parse(&p, json, strlen(json), t, MAX_TOKENS);
    if (r < 0)
        FATAL_NON_RECOVERABLE("Invalid config file (JSON parsing error or too large)");

    if (t[0].type != JSMN_OBJECT)
        FATAL_NON_RECOVERABLE("Invalid config file (expected JSON object)");

    for (size_t i = 0; i < r; ++i) {
        if (EQ_STR(json, &t[i], "services"))) {

        }
    }

    return config;
}

Config* config_json_load(const char* path)
{
    size_t sz;
    char* json = (char *) whole_file_read(path, &sz);
    if (json == NULL)
        THROW_PROPAGATE_NULL
    Config* config = config_json_from_string(json);
    free(json);
    return config;
}

void config_json_free(Config* config)
{
    for (size_t i = 0; i < config->services_sz; ++i)
        free(config->services[i].name);
    free(config->services);
    free(config);
}
