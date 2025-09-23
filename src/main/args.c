#include "args.h"

#include <getopt.h>
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

    while (true) {
        static struct option long_options[] = {
            { "help",          no_argument,       NULL, 'h' },
            { "data-dir",      required_argument, NULL, 'D' },
            { "verbose",       no_argument,       NULL, 'v' },

            // used to define services and their characteristics
            { "service",       required_argument, NULL, 's' },
            { "log-color",     required_argument, NULL, 'c' },
        };
        int idx;
        int c = getopt_long(argc, argv, "hs:D:P:wf:c:v", long_options, &idx);
        if (c == -1)
            break;

        switch (c) {
            case 'h': print_help(); break;
            case 'D': args.data_dir = strdup(optarg); break;
            case 's': args.service = strdup(optarg); break;
            case 'c': args.logging_color = optarg; break;
            case 'v': args.verbose = true; break;
            case '?': break;
            default: err_non_recoverable("Unexpected error while parsing arguments");
        }
    }

    if (optind < argc)
        err_non_recoverable("Argument %s not supported.", argv[optind]);

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
