#include "service.hh"

#include <fstream>
#include <filesystem>
namespace fs = std::filesystem;

void Service::create_file_if_it_doesnt_exist(std::string const& path, CompressedData const& data)
{
    if (!fs::exists(path)) {
        std::string file_text = data.uncompress_text();
        fs::create_directory(std::filesystem::path(path).parent_path());
        std::ofstream out(path, std::ios::out | std::ios::trunc);
        if (!out)
            throw std::runtime_error("Could not create file " + path);
        out << file_text;
    }
}
