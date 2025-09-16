#include "gz.hh"

#include <cstdlib>
#include <fstream>
#include <stdexcept>
#include <miniz.h>

namespace gz {

std::vector<uint8_t> gzip(std::vector<uint8_t> const& data)
{
    std::vector<uint8_t> compressed;
    unsigned long compressed_sz = data.size()*2+18;
    compressed.reserve(10);
    compressed.push_back(0x1f);
    compressed.push_back(0x8b);
    compressed.push_back(0x8);
    for (size_t i = 0; i < 6; ++i) compressed.push_back(0);
    compressed.push_back(0xff);
    compressed.resize(compressed_sz + 10);

    mz_stream stream;
    memset(&stream, 0, sizeof(stream));

    stream.next_in = (unsigned char *) data.data();
    stream.avail_in = (mz_uint32) data.size();
    stream.next_out = &compressed[10];
    stream.avail_out = (mz_uint32) compressed_sz;

    int result = mz_deflateInit2(&stream, MZ_BEST_COMPRESSION,
                 MZ_DEFLATED, -MZ_DEFAULT_WINDOW_BITS, 9,
                 MZ_DEFAULT_STRATEGY);

    if(result == MZ_OK)
    {
        result = mz_deflate(&stream, MZ_FINISH);
        if(result == MZ_STREAM_END)
        {
            compressed.resize(stream.total_out + 10);
            uLong crc = mz_crc32(MZ_CRC32_INIT, (mz_uint8*) (data.data()),
                                 data.size());
            compressed.push_back(crc & 0xff);
            compressed.push_back((crc >> 8) & 0xff);
            compressed.push_back((crc >> 16) & 0xff);
            compressed.push_back((crc >> 24) & 0xff);
            uLong len = data.size();
            compressed.push_back(len & 0xff);
            compressed.push_back((len >> 8) & 0xff);
            compressed.push_back((len >> 16) & 0xff);
            compressed.push_back((len >> 24) & 0xff);
        }
        mz_deflateEnd(&stream);
        return compressed;
    }

    throw std::runtime_error("Error compressing binary");
}

std::vector<uint8_t> gunzip(std::vector<uint8_t> const& compressed)
{
    if (compressed.size() < 18)
        throw std::runtime_error("Invalid gzip: too short");

    // --- Step 1: parse header ---
    if (!(compressed[0] == 0x1f && compressed[1] == 0x8b && compressed[2] == 0x08))
        throw std::runtime_error("Invalid gzip: bad magic/header");

    // Our compressor always wrote: ID1 ID2 CM FLG MTIME(4) XFL OS
    // so header is always 10 bytes.
    size_t header_size = 10;
    size_t footer_size = 8;

    // --- Step 2: prepare for inflate ---
    mz_stream stream;
    memset(&stream, 0, sizeof(stream));

    stream.next_in = (unsigned char*)&compressed[header_size];
    stream.avail_in = (mz_uint32)(compressed.size() - header_size - footer_size);

    // Guess decompressed size from ISIZE in footer:
    uLong isize =
        (compressed[compressed.size()-4]) |
        (compressed[compressed.size()-3] << 8) |
        (compressed[compressed.size()-2] << 16) |
        (compressed[compressed.size()-1] << 24);

    std::vector<uint8_t> out;
    out.resize(isize);

    stream.next_out = out.data();
    stream.avail_out = (mz_uint32)out.size();

    int result = mz_inflateInit2(&stream, -MZ_DEFAULT_WINDOW_BITS);
    // negative = raw DEFLATE (no zlib/gzip header)

    if (result != MZ_OK)
        throw std::runtime_error("inflateInit failed");

    result = mz_inflate(&stream, MZ_FINISH);
    if (result != MZ_STREAM_END) {
        mz_inflateEnd(&stream);
        throw std::runtime_error("inflate failed");
    }

    mz_inflateEnd(&stream);

    if (out.size() != isize)
        throw std::runtime_error("Size mismatch");

    return out;
}

std::vector<uint8_t> gunzip(uint8_t const* data, size_t sz)
{
    return gunzip(std::vector<uint8_t>(data, data + sz));
}

}
