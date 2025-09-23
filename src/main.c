#include <stdbool.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "main/args.h"
#include "file/gz.h"

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

    args_free();
    return EXIT_SUCCESS;
}
