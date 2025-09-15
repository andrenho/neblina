#ifndef GZ_HH
#define GZ_HH

#include <cstdint>
#include <vector>

namespace gz {
    std::vector<uint8_t> compress(std::vector<uint8_t> const& uncompressed_data);
    std::vector<uint8_t> uncompress(std::vector<uint8_t> const& compressed_data);
}

#endif //GZ_HH
