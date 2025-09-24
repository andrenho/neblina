#include "common.h"

#include "file/fileset.h"

#include "init.gen.inc"
#include "os/os.h"
#include "os/fs.h"
#include "os/window.h"

bool termination_requested = false;  // global

int main(int argc, char* argv[])
{
    window_init();

    // exit gracefully on CTRL+C
    os_handle_ctrl_c();

    // read and parse command-line arguments
    args_parse(argc, argv);

    if (!args.service) {
        // create initial directory setup
        if (!fs_file_exists(args.data_dir)) {
            if (!deploy_fileset(&fileset, args.data_dir)) {
                ERR("There was an error trying to deploy the initial file configuration: %s", last_error);
                return EXIT_FAILURE;
            }
            LOG("Initial directory setup deployed to '%s'", args.data_dir);
        }
        LOG("Orchestrator service initialized");
    }

    // cleanup
    window_close();
    args_free();

    return EXIT_SUCCESS;
}
