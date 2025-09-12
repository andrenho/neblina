#ifndef COMPRESSED_DATA_HH
#define COMPRESSED_DATA_HH

#include <cstdint>
#include <string>

class CompressedData {
public:
    CompressedData(uint8_t const* data, size_t uncompressed_sz, size_t compressed_sz)
        : data_(data), uncompressed_sz_(uncompressed_sz), compressed_sz_(compressed_sz) {}

    std::string uncompress_text() const;

private:
    uint8_t const* data_;
    size_t uncompressed_sz_;
    size_t compressed_sz_;
};

#endif //COMPRESSED_DATA_HH
