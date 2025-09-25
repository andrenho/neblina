#include "example.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TOKENS 2048

JSONParseResult example_json_from_string(const char* json, Example* example)
{
    jsmn_parser p;
    jsmn_init(&p);

    jsmntok_t t[MAX_TOKENS];
    int r = jsmn_parse(&p, json, strlen(json), t, MAX_TOKENS);
    if (r < 0)
        return J_PARSE_ERROR;
    if (t[0].type != JSMN_OBJECT)
        return J_INVALID_FIELD_TYPE;

    for (int i = 0; i < r; ++i) {
        // TODO - check field type
        i = json_set_str(json, t, i, &example->name, "name");
        i = json_set_int(json, t, i, &example->age, "age");
        i = json_set_double(json, t, i, &example->height, "height");
    }

    return J_OK;
}

JSONParseResult example_json_load(const char* path, Example* example)
{
    JSONParseResult r;

    char* json;
    if ((r = json_read_file(path, &json)) != J_OK) {
        free(json);
        return r;
    }
    r = example_json_from_string(json, example);
    free(json);
    return r;
}

void example_json_free(Example* example)
{
    for (size_t i = 0; i < example->obj_sz; ++i)
        free(example->obj[i].name);
    free(example->name);
    free(example->obj);
}