#include "os/fs.h"

#define _POSIX_C_SOURCE 1
#include <sys/stat.h>

#include "common.h"

bool fs_file_exists(const char* path)
{
    // TODO
}

static int mkdir_maybe_existent(const char* path)
{
    // TODO
}

bool fs_mkdir_recursive(const char* path)
{
    // TODO
}
