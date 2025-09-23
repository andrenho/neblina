#ifndef WHOLE_FILE_HH
#define WHOLE_FILE_HH

#include <cstdint>
#include <string>
#include <vector>

#include "util/filesystem.hh"

namespace whole_file {

std::vector<uint8_t> read_bytes(fs::path const& path);
std::string          read_string(fs::path const& path);
void                 write(fs::path const& path, std::vector<uint8_t> const& data);
void                 write(fs::path const& path, std::string const& data);

}

#endif //WHOLE_FILE_HH
