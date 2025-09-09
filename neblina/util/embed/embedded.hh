#ifndef EMBEDDED_HH
#define EMBEDDED_HH

#include <cstdint>
#include <string>

std::string decode_embedded_text(uint8_t const* data, size_t sz, size_t uncompressed_sz);

#endif //EMBEDDED_HH
