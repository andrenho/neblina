#ifndef ARGS_H_
#define ARGS_H_

#include <stdbool.h>

typedef struct {
    const char* program_name;
    const char* data_dir;
    const char* service;
    bool        verbose;
} Arguments;

void args_parse(int argc, char* argv[]);

extern Arguments args;

#endif
