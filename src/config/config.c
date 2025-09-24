#include "config.h"

#define JSMN_HEADER
#include <jsmn.h>

#include "common.h"
#include "file/whole_file.h"

#define MAX_TOKENS 2048

#define IS_KEY(json, tok, txt) (((tok).type == JSMN_STRING) && ((int)strlen(txt) == ((tok).end - (tok).start)) && (strncmp((json) + (tok).start, (txt), (tok).end - (tok).start))

bool config_json_from_string(const char* json, Config* config)
{
    jsmn_parser p;
    jsmn_init(&p);

    jsmntok_t t[MAX_TOKENS];
    int r = jsmn_parse(&p, json, strlen(json), t, MAX_TOKENS);
    if (r < 0)
        THROW("Invalid config file (JSON parsing error or too large)");

    if (t[0].type != JSMN_OBJECT)
        THROW("Invalid config file (expected JSON object)");

    for (size_t i = 0; i < r; ++i) {
        if (t[i].type == JSMN_STRING) {
            size_t key_sz = t[i].end - t[i].start;
            const char* key_ptr = json + t[i].start;
            if (memcmp(key_ptr, "test", key_sz) == 0) {
                size_t str_sz = t[i+1].end - t[i+1].start;
                config->hello = malloc(str_sz + 1);
                memcpy(config->hello, json + t[i+1].start, str_sz);
                config->hello[str_sz] = '\0';
            }
        }
    }

    return true;
}

bool config_json_load(const char* path, Config* config)
{
    size_t sz;
    char* json = (char *) whole_file_read(path, &sz);
    if (json == NULL)
        THROW_PROPAGATE
    int r = config_json_from_string(json, config);
    free(json);
    if (!r)
        THROW_PROPAGATE
    return true;
}

void config_json_free(Config* config)
{
    for (size_t i = 0; i < config->services_sz; ++i)
        free(config->services[i].name);
    free(config->services);
}
