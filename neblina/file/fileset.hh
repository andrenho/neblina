#ifndef FILESET_HH
#define FILESET_HH

#include <cstdint>
#include <string>
#include <unordered_map>

struct File {
    uint8_t const* contents;
    size_t         compressed_sz;
    size_t         uncompressed_sz;
    bool           is_compressed;
    std::string    etag {};

    static File uncompressed(uint8_t const* contents, size_t sz, std::string const& etag={}) { return { contents, 0, sz, false, etag }; }
    static File compressed(uint8_t const* contents, size_t sz, size_t usz, std::string const& etag={}) { return { contents, sz, usz, true, etag }; }
};

using FileSet = std::unordered_map<std::string, File>;

void deploy_fileset(FileSet const& fileset, std::string const& directory);

#endif //FILESET_HH
