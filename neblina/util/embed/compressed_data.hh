#ifndef COMPRESSED_DATA_HH
#define COMPRESSED_DATA_HH

#include <cstdint>
#include <string>

class CompressedData {
public:
    CompressedData(uint8_t const* data, size_t uncompressed_sz, size_t compressed_sz);

    std::string decode_text();
};

#endif //COMPRESSED_DATA_HH
