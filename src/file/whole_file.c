#include "whole_file.h"

#include <stdio.h>
#include <stdlib.h>

uint8_t* whole_file_read(const char* path, size_t* sz_out)
{
    FILE* f = fopen(path, "rb");
    if (!f)
        return NULL;

    fseek(f, 0, SEEK_END);
    size_t sz = ftell(f);
    rewind(f);

    uint8_t* buffer = malloc(sz);

    size_t pos = 0;
    do {
        int n = fread(&buffer[pos], 1, sz - pos, f);
        pos += n;
    } while (pos < sz);

    fclose(f);

    if (sz_out)
        *sz_out = sz;
    return buffer;
}

bool whole_file_write(const char* path, uint8_t const* data, size_t sz)
{
    FILE* f = fopen(path, "wb");
    if (!f)
        return false;

    size_t pos = 0;
    do {
        unsigned long n = fwrite(&data[pos], 1, sz - pos, f);
        pos += n;
    } while (pos < sz);

    fclose(f);
    return true;
}
