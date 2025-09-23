#include "args.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "error.h"

Arguments args;

static void print_help()
{
    printf("Usage: %s [OPTIONS]\n", args.program_name);
    printf("-D, --data-dir [PATH]           Choose data dir path\n");
    printf("-v, --verbose                   Verbose mode (for debugging)\n");
    exit(EXIT_SUCCESS);
}

void args_parse(int argc, char* argv[])
{
    args = (Arguments) {
        .program_name = argv[0],
        .data_dir = NULL,
        .logging_color = NULL,
        .service = NULL,
        .verbose = false,
    };

    typedef enum { CNONE, DATA_DIR, SERVICE, LOG_COLOR } Context;
    Context context = CNONE;

    for (int i = 1; i < argc; ++i) {
        switch (context) {
            case CNONE:
                if (strcmp(argv[i], "-D") == 0)
                    context = DATA_DIR;
                else if (strcmp(argv[i], "-s") == 0)
                    context = SERVICE;
                else if (strcmp(argv[i], "-c") == 0)
                    context = LOG_COLOR;
                else if (strcmp(argv[i], "-h") == 0)
                    print_help();
                else if (strcmp(argv[i], "-v") == 0)
                    args.verbose = true;
                else
                    err_non_recoverable("Argument %s not supported", argv[i]);
                break;
            case DATA_DIR:
                args.data_dir = strdup(argv[i]);
                context = CNONE;
                continue;
            case SERVICE:
                args.service = strdup(argv[i]);
                context = CNONE;
                continue;
            case LOG_COLOR:
                args.logging_color = strdup(argv[i]);
                context = CNONE;
                continue;
        }
    }

    if (context != CNONE)
        err_non_recoverable("Missing argument");

    if (!args.data_dir) {
        args.data_dir = malloc(1024);
        snprintf(args.data_dir, 1024, "%s/neblina", getenv("HOME"));
        args.data_dir = realloc(args.data_dir, strlen(args.data_dir) + 1);
    }
    if (!args.service)
        args.service = strdup("orchestrator");
    if (!args.logging_color)
        args.logging_color = strdup("0;30");
}

void args_free()
{
    free(args.data_dir);
    free(args.service);
    free(args.logging_color);
}
