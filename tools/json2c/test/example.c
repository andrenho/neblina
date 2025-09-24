#include "example.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "jsmn.h"

#define MAX_TOKENS 2048

JSONParseResult example_json_from_string(const char* json, Example* example)
{
    jsmn_parser p;
    int r;
    JSONParseResult res = json_init(&p, json, &r, MAX_TOKENS);
    if (r != J_OK)
        return r;

    for (int i = 0; i < r; ++i) {
    }

    return res;
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