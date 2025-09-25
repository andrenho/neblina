#include "example.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TOKENS 2048

#define CHK { if(i < 0) abort(); } // return i; }

static void example_init(Example* example)
{
    example->name = STRING_NULL;
    example->age = INT_NULL;
    example->height = DOUBLE_NULL;
    example->gender = BOOL_NULL;
    example->optional = INT_NULL;
    example->obj_sz = 0;
}

JSONParseResult example_json_from_string(const char* json, Example* example)
{
    example_init(example);

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
        i = json_set_str(json, t, i, &example->name, "name"); CHK
        i = json_set_int(json, t, i, &example->age, "age"); CHK
        i = json_set_double(json, t, i, &example->height, "height"); CHK
        i = json_set_boolean(json, t, i, &example->gender, "gender"); CHK
        i = json_set_int(json, t, i, &example->optional, "optional"); CHK
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