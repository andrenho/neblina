#include "embedded.hh"

#include <stdexcept>
#include <zlib.h>

#include <string>
using namespace std::string_literals;

std::string decode_embedded_text(uint8_t const* data, size_t sz, size_t uncompressed_sz)
{
    std::string output(uncompressed_sz, 0);

    z_stream strm = {};
    strm.next_in = (Bytef *) data;
    strm.avail_in = sz;
    strm.next_out = (Bytef *) output.data();
    strm.avail_out = uncompressed_sz;

    // gzip header support
    if (int ret = inflateInit2(&strm, 16 + MAX_WBITS) != Z_OK)
        throw std::runtime_error("Error uncompressing: "s + zError(ret));

    if (int ret = inflate(&strm, Z_FINISH) != Z_STREAM_END)
        throw std::runtime_error("Error uncompressing: "s + zError(ret));

    inflateEnd(&strm);

    return output;
}
