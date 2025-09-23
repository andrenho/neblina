#include "args.h"

#include <getopt.h>
#include <stddef.h>

void args_parse(int argc, char* argv[])
{
    while (true) {
        static struct option long_options[] = {
            { "help",          no_argument,       NULL, 'h' },
            { "data-dir",      required_argument, NULL, 'D' },
            { "verbose",       no_argument,       NULL, 'v' },

            // used to define services and their characteristics
            { "service",       required_argument, NULL, 's' },
            { "port",          required_argument, NULL, 'P' },
            { "open-to-world", no_argument,       NULL, 'w' },
            { "frequency",     required_argument, NULL, 'f' },
            { "log-color",     required_argument, NULL, 'c' },
        };
        int idx;
        int c = getopt_long(argc, argv, "hs:D:P:wf:c:v", long_options, &idx);
        if (c == -1)
            break;

        switch (c) {
            case 'h': print_help(argv[0]); break;
            case 'D': data_dir = optarg; break;
            case 'P': port = (int) strtol(optarg, NULL, 10); break;
            case 'w': open_to_world = true; break;
            case 'f': frequency = std::chrono::milliseconds(strtoll(optarg, NULL, 10)); break;
            case 's': service = optarg; break;
            case 'c': logging_color = optarg; break;
            case 'v': verbose = true; break;
            default: throw NonRecoverableException("Unexpected error while parsing arguments");
        }
    }

    if (optind < argc)
        throw NonRecoverableException("Argument "s + argv[optind] + " not supported.");
}
