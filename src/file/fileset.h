#ifndef FILESET_HH
#define FILESET_HH

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

typedef struct {
    const char*    name;
    uint8_t const* contents;
    size_t         compressed_sz;
    size_t         uncompressed_sz;
    const char*    etag;
} NFile;

typedef struct {
    NFile const** files;
    size_t        n_files;
} NFileSet;

bool file_exists(const char* path);
bool deploy_file(NFile const* file, const char* path);
bool deploy_fileset(NFileSet const* fileset, const char* path);

#endif //FILESET_HH
