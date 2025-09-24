#include <jsmn.h>   // implementation

#include "common.h"
#include "file/fileset.h"
#include "config/config.h"
#include "os/os.h"
#include "os/fs.h"
#include "os/window.h"

#include "init.gen.inc"

bool termination_requested = false;  // global
Config main_config;

int main(int argc, char* argv[])
{
    window_init();

    // exit gracefully on CTRL+C
    os_handle_ctrl_c();

    // read and parse command-line arguments
    args_parse(argc, argv);

    // create initial directory setup
    if (!fs_file_exists(args.data_dir) && !args.service) {
        if (!deploy_fileset(&fileset, args.data_dir)) {
            ERR("There was an error trying to deploy the initial file configuration: %s", last_error);
            return EXIT_FAILURE;
        }
        LOG("Initial directory setup deployed to '%s'", args.data_dir);
    }

    // load config file
    {
        char config_filename[PATH_MAX]; snprintf(config_filename, PATH_MAX, "%s/config/config.json", args.data_dir);
        if (!config_json_load(config_filename, &main_config))
            FATAL_NON_RECOVERABLE("Error loading config file: %s", last_error);
    }

    // cleanup
    config_json_free(&main_config);
    window_close();
    args_free();

    return EXIT_SUCCESS;
}
