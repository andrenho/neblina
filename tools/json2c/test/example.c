#include "example.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CHK(i) { if(i < 0) abort(); } // return i; }

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
    jsmn_parser p;

    example_init(example);

    // count tokens
    jsmn_init(&p);
    size_t json_sz = strlen(json);
    int n_tokens = jsmn_parse(&p, json, json_sz, NULL, 0);
    if (n_tokens < 0)
        return J_PARSE_ERROR;

    // parse JSON
    jsmntok_t t[n_tokens];
    jsmn_init(&p);
    int r = jsmn_parse(&p, json, json_sz, t, n_tokens);
    if (t[0].type != JSMN_OBJECT)
        return J_INVALID_FIELD_TYPE;

    for (int i = 0; i < r; ++i) {
        int a = 0;

        i = json_field_str(json, t, i, &example->name, "name"); CHK(i)
        i = json_field_int(json, t, i, &example->age, "age"); CHK(i)
        i = json_field_double(json, t, i, &example->height, "height"); CHK(i)
        i = json_field_boolean(json, t, i, &example->gender, "gender"); CHK(i)
        i = json_field_int(json, t, i, &example->optional, "optional"); CHK(i)

        // items
        if ((a = json_field_array(json, t, i, "items")) >= 0) {
            example->items_sz = a;
            example->items = malloc(a * sizeof(int));
            for (int j = 0; j < a; ++j) {
                JSONParseResult v = json_int(json, t, i + j + 1, &example->items[j]); CHK(v)
            }
            i += a + 1;
        }

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