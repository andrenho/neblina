#include "common.h"
#include "file/fileset.h"
#include "os/os.h"
#include "os/fs.h"
#include "os/window.h"
#include "config/config.h"
#include "file/whole_file.h"
#include "service/orchestrator.h"

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
    DBG("Initializing in DBG mode");

    // create initial directory setup
    DBG("Data dir: %s", args.data_dir);
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
        char* json = (char *) whole_file_read(config_filename, NULL);
        if (!json)
            FATAL_NON_RECOVERABLE("Could not open config file: %s", last_error);
        if (!config_load(json, &main_config))
            FATAL_NON_RECOVERABLE("Error parsing config file - not valid JSON or in unexpected format");
        free(json);
    }

    // start services
    if (!args.service)
        orchestrator_start();

    // cleanup
    window_close();
    args_free();

    return EXIT_SUCCESS;
}
