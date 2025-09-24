#include <stdbool.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>

#include "main/args.h"
#include "file/fileset.h"
#include "util/logs.h"

#include "init.gen.inc"
#include "main/error.h"

bool termination_requested = false;

void handle_sigint(int signum)
{
    (void) signum;
    termination_requested = true;
}

int main(int argc, char* argv[])
{
    // signal to catch CTRL+C and exit gracefully
    signal(SIGINT, handle_sigint);
    signal(SIGTERM, handle_sigint);

    // read and parse command-line arguments
    args_parse(argc, argv);

    // create initial directory setup
    if (!file_exists(args.data_dir) && !args.service) {
        if (!deploy_fileset(&fileset, args.data_dir)) {
            ERR("There was an error trying to deploy the initial file configuration: %s", last_error);
            return EXIT_FAILURE;
        }
        LOG("Initial directory setup deployed to '%s'", args.data_dir);
    }

    args_free();
    fflush(stdout);
    fflush(stderr);
    return EXIT_SUCCESS;
}
