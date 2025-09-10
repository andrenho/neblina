#ifndef CONFIG_HH
#define CONFIG_HH

#include <string>

class Config {
public:
    explicit Config(std::string const& config_file_path, std::string const& filename,
        uint8_t const* compressed_config_file=nullptr, size_t compressed_config_file_sz=0, size_t uncompressed_config_file_sz=0);
    virtual ~Config() = default;

    virtual Config const& load() = 0;

protected:
    void create_config_file_if_it_doesnt_exist(std::string const& path);

    std::string    config_filename_;
    uint8_t const* compressed_config_file_;
    size_t         compressed_config_file_sz_;
    size_t         uncompressed_config_file_sz_;
};

#endif //CONFIG_HH
