#include "config.hh"

#include <fstream>
#include <filesystem>
namespace fs = std::filesystem;

#include "util/embed/embedded.hh"

Config::Config(std::string const& config_file_path, std::string const& filename,
                uint8_t const* compressed_config_file, size_t compressed_config_file_sz, size_t uncompressed_config_file_sz)
    : config_filename_(config_file_path + "/" + filename),
      compressed_config_file_(compressed_config_file), compressed_config_file_sz_(compressed_config_file_sz), uncompressed_config_file_sz_(uncompressed_config_file_sz)
{
    create_config_file_if_it_doesnt_exist(config_file_path);
}

void Config::create_config_file_if_it_doesnt_exist(std::string const& path)
{
    if (compressed_config_file_ && !fs::exists(config_filename_)) {
        std::string default_config_file = decode_embedded_text(compressed_config_file_, compressed_config_file_sz_, uncompressed_config_file_sz_);
        fs::create_directory(path);
        std::ofstream out(config_filename_, std::ios::out | std::ios::trunc);
        if (!out)
            throw std::runtime_error("Could not create default config file");
        out << default_config_file;
    }
}