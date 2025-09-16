#ifndef WHOLE_FILE_HH
#define WHOLE_FILE_HH

#include <cstdint>
#include <string>
#include <vector>

namespace whole_file {

std::vector<uint8_t> read_bytes(const std::string& path);
std::string          read_string(const std::string& path);
void                 write(const std::string& path, const std::vector<uint8_t>& data);
void                 write(const std::string& path, const std::string& data);

}

#endif //WHOLE_FILE_HH
