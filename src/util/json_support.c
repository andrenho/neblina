#include "jsmn.h"

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

JSONParseResult json_int(const char* json, jsmntok_t const* t, int i, int* out)
{
    char s = json[t[i+1].start];
    if (t[i+1].type == JSMN_PRIMITIVE && s == 'n') {
        *out = INT_NULL;
        return J_OK;
    }
    if (t[i+1].type != JSMN_PRIMITIVE || (s != '_' && (s < '0' || s > '9')))
        return J_INVALID_FIELD_TYPE;
    size_t str_sz = t[i+1].end - t[i+1].start;
    char nfield[str_sz + 1]; memcpy(nfield, json + t[i+1].start, str_sz); nfield[str_sz] = '\0';
    *out = (int) strtol(nfield, NULL, 10);
    return J_OK;
}

int json_field_str(const char* json, jsmntok_t const* t, int i, char** obj_field, const char* field_name)
{
    if (t[i].type == JSMN_STRING) {
        const char* key_ptr = json + t[i].start;
        if (memcmp(key_ptr, field_name, strlen(field_name)) == 0) {
            char s = json[t[i+1].start];
            if (t[i+1].type == JSMN_PRIMITIVE && s == 'n') {
                *obj_field = STRING_NULL;
                return i + 1;
            }
            if (t[i+1].type != JSMN_STRING)
                return J_INVALID_FIELD_TYPE;
            size_t str_sz = t[i+1].end - t[i+1].start;
            *obj_field = malloc(str_sz + 1);
            memcpy(*obj_field, json + t[i+1].start, str_sz);
            (*obj_field)[str_sz] = '\0';
            return i + 1;
        }
    }
    return i;
}

int json_field_int(const char* json, jsmntok_t const* t, int i, int* obj_field, const char* field_name)
{
    if (t[i].type == JSMN_STRING) {
        const char* key_ptr = json + t[i].start;
        if (memcmp(key_ptr, field_name, strlen(field_name)) == 0) {
            char s = json[t[i+1].start];
            if (t[i+1].type == JSMN_PRIMITIVE && s == 'n') {
                *obj_field = INT_NULL;
                return i + 1;
            }
            if (t[i+1].type != JSMN_PRIMITIVE || (s != '_' && (s < '0' || s > '9')))
                return J_INVALID_FIELD_TYPE;
            size_t str_sz = t[i+1].end - t[i+1].start;
            char nfield[str_sz + 1]; memcpy(nfield, json + t[i+1].start, str_sz); nfield[str_sz] = '\0';
            *obj_field = (int) strtol(nfield, NULL, 10);
            return i + 1;
        }
    }
    return i;
}

int json_field_double(const char* json, jsmntok_t const* t, int i, double* obj_field, const char* field_name)
{
    if (t[i].type == JSMN_STRING) {
        const char* key_ptr = json + t[i].start;
        if (memcmp(key_ptr, field_name, strlen(field_name)) == 0) {
            char s = json[t[i+1].start];
            if (t[i+1].type == JSMN_PRIMITIVE && s == 'n') {
                *obj_field = DOUBLE_NULL;
                return i + 1;
            }
            if (t[i+1].type != JSMN_PRIMITIVE || (s != '_' && (s < '0' || s > '9')))
                return J_INVALID_FIELD_TYPE;
            size_t str_sz = t[i+1].end - t[i+1].start;
            char nfield[str_sz + 1]; memcpy(nfield, json + t[i+1].start, str_sz); nfield[str_sz] = '\0';
            *obj_field = strtod(nfield, NULL);
            return i + 1;
        }
    }
    return i;
}

int json_field_boolean(const char* json, jsmntok_t const* t, int i, bool* obj_field, const char* field_name)
{
    if (t[i].type == JSMN_STRING) {
        const char* key_ptr = json + t[i].start;
        if (memcmp(key_ptr, field_name, strlen(field_name)) == 0) {
            char s = json[t[i+1].start];
            if (t[i+1].type == JSMN_PRIMITIVE && s == 'n') {
                *obj_field = BOOL_NULL;
                return i + 1;
            }
            if (t[i+1].type != JSMN_PRIMITIVE && s != 't' && s != 'f')
                return J_INVALID_FIELD_TYPE;
            *obj_field = (s == 't');
            return i + 1;
        }
    }
    return i;
}

int json_field_array(const char* json, jsmntok_t const* t, int i, const char* field_name)
{
    if (t[i].type == JSMN_STRING) {
        const char* key_ptr = json + t[i].start;
        if (memcmp(key_ptr, field_name, strlen(field_name)) != 0)
            return -1;
    } else {
        return -1;
    }
    if (t[i+1].type != JSMN_ARRAY)
        return -1;
    return t[i+1].size;
}
