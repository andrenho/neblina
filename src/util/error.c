#include "error.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#define LAST_ERROR_SZ 8128
char last_error[LAST_ERROR_SZ];

void FATAL_NON_RECOVERABLE(const char* fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, ".\n");
    va_end(ap);
    exit(NON_RECOVERABLE_ERROR);
}

void FATAL(const char* fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, ".\n");
    va_end(ap);
    exit(EXIT_FAILURE);
}
