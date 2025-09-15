#include "gz.hh"

#include <cstdlib>
#include <fstream>
#include <stdexcept>
#include <miniz.h>

namespace gz {

std::vector<uint8_t> compress(std::vector<uint8_t> const& data)
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
            std::ofstream some_file("test.txt.gz");
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

std::vector<uint8_t> uncompress(std::vector<uint8_t> const& compressed_data)
{

}

}
