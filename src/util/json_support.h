#ifndef NEBLINA_JSON_SUPPORT_H
#define NEBLINA_JSON_SUPPORT_H

#include <float.h>
#include <limits.h>
#include <stdbool.h>
#include <stddef.h>

#define JSMN_HEADER
#include "jsmn.h"

typedef unsigned char JBool;

typedef enum {
    J_OK, J_CANT_OPEN_FILE, J_PARSE_ERROR, J_INVALID_FIELD_TYPE,
} JSONParseResult;

#define STRING_NULL  NULL
#define OBJ_NULL     NULL
#define INT_NULL     INT_MIN
#define DOUBLE_NULL  DBL_MIN
#define BOOL_NULL    50

JSONParseResult json_read_file(const char* path, char** json);

JSONParseResult json_init(jsmn_parser* p, const char* json, int* r, size_t max_tokens);


#endif //NEBLINA_JSON_SUPPORT_H
