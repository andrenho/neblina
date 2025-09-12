#include "compressed_data.hh"

#include <stdexcept>
#include <zlib.h>

#include <string>
using namespace std::string_literals;

std::string CompressedData::uncompress_text() const
{
    std::string output(uncompressed_sz_, 0);

    z_stream strm = {};
    strm.next_in = (Bytef *) data_;
    strm.avail_in = compressed_sz_;
    strm.next_out = (Bytef *) output.data();
    strm.avail_out = uncompressed_sz_;

    // gzip header support
    if (int ret = inflateInit2(&strm, 16 + MAX_WBITS) != Z_OK)
        throw std::runtime_error("Error uncompressing: "s + zError(ret));

    if (int ret = inflate(&strm, Z_FINISH) != Z_STREAM_END)
        throw std::runtime_error("Error uncompressing: "s + zError(ret));

    inflateEnd(&strm);

    return output;
}
