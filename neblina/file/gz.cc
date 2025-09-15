#include "gz.hh"

#include <fstream>
#include <stdexcept>
#include <miniz.h>

namespace gz {

std::vector<uint8_t> compress(std::vector<uint8_t> const& data)
{
    std::string sDeflated = "";
    unsigned long iDeflated = data.size()*2+18;
    sDeflated.reserve(iDeflated);
    sDeflated.append("\x1f\x8b\x08\0\0\0\0\0\0\xff", 10);

    mz_stream stream;
    memset(&stream, 0, sizeof(stream));

    stream.next_in = (unsigned char *) data.data();
    stream.avail_in = (mz_uint32) data.size();
    stream.next_out = (unsigned char *) (sDeflated.data() + 10);
    stream.avail_out = (mz_uint32) iDeflated;

    int result = mz_deflateInit2(&stream, MZ_BEST_COMPRESSION,
                 MZ_DEFLATED, -MZ_DEFAULT_WINDOW_BITS, 9,
                 MZ_DEFAULT_STRATEGY);

    if(result == MZ_OK)
    {
        result = mz_deflate(&stream, MZ_FINISH);
        if(result == MZ_STREAM_END)
        {
            std::ofstream some_file("test.txt.gz");
            some_file.write(sDeflated.data(), stream.total_out + 10);
            uLong crc = mz_crc32(MZ_CRC32_INIT, (mz_uint8*) (data.data()),
                                 data.size());
            some_file.write((const char*) &crc, 4);
            uLong len = data.size();
            some_file.write((const char*) &len, 4);
        }
        mz_deflateEnd(&stream);
    }

    return {};
}

std::vector<uint8_t> uncompress(std::vector<uint8_t> const& compressed_data)
{

}

}
