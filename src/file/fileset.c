#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <sys/stat.h>

#ifdef _WIN32
#include <direct.h>
#define mkdir(path,mode) _mkdir(path)
#define PATH_SEP '\\'
#else
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>

#define PATH_SEP '/'
#endif

#include "fileset.h"
#include "gz.h"
#include "whole_file.h"
#include "main/error.h"

int mkdir_recursive(const char *path) {
    char tmp[1024];
    snprintf(tmp, sizeof(tmp), "%s", path);
    size_t len = strlen(tmp);

    if (len == 0) return -1;

    // strip trailing separators
    while (len > 1 && (tmp[len-1] == '/' || tmp[len-1] == '\\'))
        tmp[--len] = '\0';

    for (char *p = tmp + 1; *p; p++) {
        if (*p == '/' || *p == '\\') {
            *p = '\0';
            mkdir(tmp, 0755);
            *p = PATH_SEP;
        }
    }
    return mkdir(tmp, 0755);
}

bool file_exists(const char* path)
{
#if _WIN32
    struct _stat st;
    return _stat(path, &st) == 0;
#else
    struct stat st;
    return stat(path, &st) == 0;
#endif
}

bool deploy_file(NFile const* file, const char* path)
{
    char filename[8128]; snprintf(filename, sizeof filename, "%s/%s", path, file->name);
    char filepath[8128]; strcpy(filepath, filename); strrchr(filepath, '/')[0] = '\0';

    if (mkdir_recursive(filepath) != 0)
        THROW_ERRNO

    uint8_t* data = (uint8_t *) file->contents;
    size_t sz = file->uncompressed_sz;
    if (file->compressed_sz != 0) {
        data = gunzip(data, sz, &sz);
        if (!data)
            THROW_PROPAGATE
    }

    if (!whole_file_write(filename, data, sz))
        THROW_PROPAGATE

    if (file->compressed_sz != 0)
        free(data);

    return true;
}

bool deploy_fileset(NFileSet const* fileset, const char* path)
{
    for (size_t i = 0; i < fileset->n_files; ++i)
        if (!deploy_file(fileset->files[i], path))
            THROW_PROPAGATE
    return true;
}