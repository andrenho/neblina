#ifndef NEBLINA_JSON_SUPPORT_H
#define NEBLINA_JSON_SUPPORT_H

#define JSMN_HEADER
#include "jsmn.h"

#include <float.h>
#include <limits.h>
#include <stdbool.h>
#include <stddef.h>

typedef unsigned char JBool;

typedef enum {
    J_OK, J_CANT_OPEN_FILE = -1, J_PARSE_ERROR = -2, J_INVALID_FIELD_TYPE = -3,
} JSONParseResult;

#define STRING_NULL  NULL
#define OBJ_NULL     NULL
#define INT_NULL     INT_MIN
#define DOUBLE_NULL  DBL_MIN
#define BOOL_NULL    50

JSONParseResult json_read_file(const char* path, char** json);

int json_set_str(const char* json, jsmntok_t const* t, int i, char** obj_field, const char* field_name);
int json_set_int(const char* json, jsmntok_t const* t, int i, int* obj_field, const char* field_name);
int json_set_double(const char* json, jsmntok_t const* t, int i, double* obj_field, const char* field_name);

#endif //NEBLINA_JSON_SUPPORT_H
