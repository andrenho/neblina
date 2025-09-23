#include "logs.h"

#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>

#include "main/args.h"

void DBG(const char* fmt, ...)
{
    if (args.verbose)
        return;
    va_list ap;
    va_start(ap, fmt);
    printf("%s%13s: ", args.logging_color, args.service);
    vfprintf(stdout, fmt, ap);
    printf("\e[0m\n");
    va_end(ap);
}

void LOG(const char* fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    printf("%s%13s: ", args.logging_color, args.service);
    vfprintf(stdout, fmt, ap);
    printf("\e[0m\n");
    va_end(ap);
}

void ERR(const char* fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    fprintf(stderr, "%s: ", args.service);
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, "\n");
    va_end(ap);
}