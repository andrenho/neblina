#include "fileset.h"

static NFile file = {
        .name = "hello.txt",
        .contents = (uint8_t const[]) { 0, 1, 2, 3 },
        .compressed_sz = 0,
        .uncompressed_sz = 16,
};

static NFileSet fileset = {
        .files = (NFile const*[]) { &file, &file },
        .n_files = 2,
};