#include "error.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

void err_non_recoverable(const char* fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, ".\n");
    va_end(ap);
    exit(NON_RECOVERABLE_ERROR);
}


void err(const char* fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, ".\n");
    va_end(ap);
    exit(EXIT_FAILURE);
}
