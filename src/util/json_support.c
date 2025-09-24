#include "json_support.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

JSONParseResult json_read_file(const char* path, char** json)
{
    FILE* f = fopen(path, "rb");
    if (!f)
        return J_CANT_OPEN_FILE;

    fseek(f, 0, SEEK_END);
    size_t sz = ftell(f);
    rewind(f);

    *json = malloc(sz);

    size_t pos = 0;
    do {
        int n = fread(&(*json)[pos], 1, sz - pos, f);
        pos += n;
    } while (pos < sz);

    fclose(f);

    return J_OK;
}

JSONParseResult json_init(jsmn_parser* p, const char* json, int* r, size_t max_tokens)
{
    jsmn_init(p);

    jsmntok_t t[max_tokens];
    *r = jsmn_parse(p, json, strlen(json), t, max_tokens);
    if (*r < 0)
        return J_PARSE_ERROR;
    if (t[0].type != JSMN_OBJECT)
        return J_INVALID_FIELD_TYPE;

    return J_OK;
}
