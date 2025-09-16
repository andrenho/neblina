#ifndef GZ_HH
#define GZ_HH

#include <cstdint>
#include <vector>

namespace gz {

std::vector<uint8_t> gzip(std::vector<uint8_t> const& uncompressed_data);
std::vector<uint8_t> gunzip(std::vector<uint8_t> const& compressed_data);

}

#endif //GZ_HH
