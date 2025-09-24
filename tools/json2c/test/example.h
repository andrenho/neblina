#ifndef NEBLINA_EXAMPLE_H
#define NEBLINA_EXAMPLE_H

#include "util/json_support.h"

typedef struct {
    int  number;
    bool is_null;
} Example_Detail;

typedef struct {
    const char*    name;
    Example_Detail detail;
} Example_Obj;

typedef struct {
    char*        name;
    int          age;
    double       height;
    bool         gender;
    int          optional;
    Example_Obj* obj;
    size_t       obj_sz;
} Example;

bool example_json_from_string(const char* json, Example* example);
bool example_json_load(const char* path, Example* example);
void example_json_free(Example* example);

#endif //NEBLINA_EXAMPLE_H
