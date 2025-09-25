#ifndef ARGS_H_
#define ARGS_H_

#include <stdbool.h>

typedef struct {
    char* program_name;
    char* data_dir;
    char* service;
    int   logging_color;
    bool  verbose;
    int   port;
    bool  open_to_world;
} Arguments;

void args_parse(int argc, char* argv[]);
void args_free();

extern Arguments args;

#endif
