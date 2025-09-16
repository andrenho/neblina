#include "whole_file.hh"

#include <fstream>
#include <stdexcept>

namespace whole_file {

std::vector<uint8_t> read_bytes(const std::string& path)
{
    std::ifstream file(path, std::ios::binary | std::ios::ate);
    if (!file) throw std::runtime_error("Cannot open file: " + path);

    std::streamsize size = file.tellg();
    file.seekg(0);

    std::vector<uint8_t> buffer(size);
    if (!file.read(reinterpret_cast<char*>(buffer.data()), size))
        throw std::runtime_error("Error reading file: " + path);

    return buffer;
}

std::string read_string(const std::string& path)
{
    auto bytes = read_bytes(path);
    return {reinterpret_cast<const char*>(bytes.data()), bytes.size()};
}

void write(const std::string& path, const std::vector<uint8_t>& data)
{
    std::ofstream file(path, std::ios::binary);
    if (!file) throw std::runtime_error("Cannot open file: " + path);

    if (!file.write(reinterpret_cast<const char*>(data.data()), data.size()))
        throw std::runtime_error("Error writing file: " + path);
}

void write(const std::string& path, const std::string& data)
{
    write(path,
        std::vector<uint8_t> {reinterpret_cast<const uint8_t*>(data.data()),
         reinterpret_cast<const uint8_t*>(data.data()) + data.size()});
}

}