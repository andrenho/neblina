#include "compressed_data.hh"

#include <stdexcept>
#include <miniz.h>

#include <string>
using namespace std::string_literals;

std::string CompressedData::uncompress_text() const
{
    std::string output(uncompressed_sz_ + 1, 0);

    unsigned long u = uncompressed_sz_;
    int cmp_status = uncompress((unsigned char *) output.data(), &u, data_, compressed_sz_);

    if (cmp_status != Z_OK)
        throw std::runtime_error("Error uncompressing: "s + zError(cmp_status));

    return output;
}
