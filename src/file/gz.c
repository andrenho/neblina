#include "gz.h"
#include "main/error.h"

#include <stdlib.h>

#include <miniz.h>

uint8_t* gzip(uint8_t const* uncompressed, size_t usz, size_t* csz)
{
    unsigned long compressed_sz = usz * 2 + 18;
    uint8_t* compressed = calloc(1, compressed_sz);

    // gzip header
    compressed[0] = 0x1f;
    compressed[1] = 0x8b;
    compressed[2] = 0x08;
    compressed[8] = 0xff;

    // compress data
    mz_stream stream;
    memset(&stream, 0, sizeof(stream));

    stream.next_in = uncompressed;
    stream.avail_in = usz;
    stream.next_out = &compressed[10];
    stream.avail_out = compressed_sz - 10;

    int result = mz_deflateInit2(&stream, MZ_BEST_COMPRESSION,
                 MZ_DEFLATED, -MZ_DEFAULT_WINDOW_BITS, 9,
                 MZ_DEFAULT_STRATEGY);

    if(result == MZ_OK)
    {
        result = mz_deflate(&stream, MZ_FINISH);
        if(result == MZ_STREAM_END)
        {
            // compression successful, resize are and add footer
            size_t end = stream.total_out + 10;
            uLong crc = mz_crc32(MZ_CRC32_INIT, uncompressed, usz);
            compressed[end] = crc & 0xff;
            compressed[end+1] = (crc >> 8) & 0xff;
            compressed[end+2] = (crc >> 16) & 0xff;
            compressed[end+3] = (crc >> 24) & 0xff;
            uLong len = usz;
            compressed[end+4] = len & 0xff;
            compressed[end+5] = (len >> 8) & 0xff;
            compressed[end+6] = (len >> 16) & 0xff;
            compressed[end+7] = (len >> 24) & 0xff;
            compressed = realloc(compressed, end + 8);
            *csz = end + 8;
        } else {
            THROW_NULL("Error compressing binary: %s", mz_error(result))
        }
        mz_deflateEnd(&stream);
        return compressed;
    }

    THROW_NULL("Error compressing binary: %s", mz_error(result))
}

uint8_t* gunzip(uint8_t const* compressed, size_t csz, size_t* usz)
{
    if (csz < 18)
        THROW_NULL("Invalid gzip: too short")

    // --- Step 1: parse header ---
    if (!(compressed[0] == 0x1f && compressed[1] == 0x8b && compressed[2] == 0x08))
        THROW_NULL("Invalid gzip: bad magic/header")

    // Our compressor always wrote: ID1 ID2 CM FLG MTIME(4) XFL OS
    // so header is always 10 bytes.
    size_t header_size = 10;
    size_t footer_size = 8;

    // --- Step 2: prepare for inflate ---
    mz_stream stream;
    memset(&stream, 0, sizeof(stream));

    stream.next_in = &compressed[header_size];
    stream.avail_in = csz - header_size - footer_size;

    // Guess decompressed size from ISIZE in footer:
    *usz = (compressed[csz-4]) | (compressed[csz-3] << 8) | (compressed[csz-2] << 16) | (compressed[csz-1] << 24);

    uint8_t* out = calloc(1, *usz);

    stream.next_out = out;
    stream.avail_out = *usz;

    int result = mz_inflateInit2(&stream, -MZ_DEFAULT_WINDOW_BITS);
    // negative = raw DEFLATE (no zlib/gzip header)

    if (result != MZ_OK)
        THROW_NULL("decompression failed: %s", mz_error(result))

    result = mz_inflate(&stream, MZ_FINISH);
    if (result != MZ_STREAM_END) {
        mz_inflateEnd(&stream);
        THROW_NULL("decompression failed: %s", mz_error(result))
    }

    mz_inflateEnd(&stream);
    return out;
}
