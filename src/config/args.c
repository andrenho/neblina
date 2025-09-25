#include "args.h"

#include "common.h"
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
        .logging_color = 0,
        .service = NULL,
        .verbose = false,
    };

    typedef enum { CNONE, DATA_DIR, SERVICE, LOG_COLOR, PORT } Context;
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
                else if (strcmp(argv[i], "-P") == 0)
                    context = PORT;
                else if (strcmp(argv[i], "-w") == 0)
                    args.open_to_world = true;
                else if (strcmp(argv[i], "-v") == 0)
                    args.verbose = true;
                else if (strcmp(argv[i], "-h") == 0)
                    print_help();
                else
                    FATAL_NON_RECOVERABLE("Argument %s not supported", argv[i]);
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
                args.logging_color = (int) strtol(argv[i], NULL, 10);
                context = CNONE;
                continue;
            case PORT:
                args.port = (int) strtol(argv[i], NULL, 10);
                context = CNONE;
        }
    }

    if (context != CNONE)
        FATAL_NON_RECOVERABLE("Missing argument");

    if (!args.data_dir) {
#ifdef _WIN32
        args.data_dir = "./neblina";
#else
        args.data_dir = malloc(1024);
        snprintf(args.data_dir, 1024, "%s/neblina", getenv("HOME"));
        args.data_dir = realloc(args.data_dir, strlen(args.data_dir) + 1);
#endif
    }
}

void args_free()
{
    free(args.data_dir);
    free(args.service);
}
