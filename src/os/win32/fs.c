#include "os/fs.h"

#define _POSIX_C_SOURCE 1
#include <sys/stat.h>
#include <direct.h>

#include "common.h"

bool fs_file_exists(const char* path)
{
    struct _stat st;
    return _stat(path, &st) == 0;
    // TODO
}

static int mkdir_maybe_existent(const char* path)
{
    int ret = _mkdir(path);
    if (ret != 0 && errno == EEXIST)
        ret = 0;
    return ret;
    // TODO
}

bool fs_mkdir_recursive(const char* path)
{
    int ret = 0;

    char* path_buf = strdup(path);
    char* slash = path_buf;

    for (;;) {
        slash = strchr(slash + 1, '/');
        if (!slash) {
            ret = mkdir_maybe_existent(path_buf);
            break;
        }
        *slash = '\0';
        ret = mkdir_maybe_existent(path_buf);
        if (ret != 0)
            break;
        *slash = '/';
    }

    free(path_buf);

    return ret == 0;
    // TODO
}
