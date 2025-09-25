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
#include <stdlib.h>

#define PATH_SEP '/'
#endif

#include "fileset.h"
#include "gz.h"
#include "whole_file.h"
#include "main/error.h"
#include "os/fs.h"

bool deploy_file(NFile const* file, const char* path)
{
    char filename[8128]; snprintf(filename, sizeof filename, "%s/%s", path, file->name);
    char filepath[8128]; strcpy(filepath, filename); strrchr(filepath, '/')[0] = '\0';

    fs_mkdir_recursive(filepath);

    uint8_t* data = (uint8_t *) file->contents;
    size_t sz = file->uncompressed_sz;
    if (file->compressed_sz != 0) {
        size_t usz;
        data = gunzip(data, file->compressed_sz, &usz);
        sz = usz;
        if (!data)
            THROW_PROPAGATE
    }

    if (!whole_file_write(filename, data, sz))
        THROW_PROPAGATE

    if (file->compressed_sz != 0)
        free(data);

    DBG("File deployed: %s", filename);

    return true;
}

bool deploy_fileset(NFileSet const* fileset, const char* path)
{
    for (size_t i = 0; i < fileset->n_files; ++i)
        if (!deploy_file(fileset->files[i], path))
            THROW_PROPAGATE
    return true;
}
