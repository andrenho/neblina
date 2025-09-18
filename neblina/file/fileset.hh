#ifndef FILESET_HH
#define FILESET_HH

#include <cstdint>
#include <string>
#include <unordered_map>

#include "util/filesystem.hh"

// File and FileSet are used by the "embed" generator to generate embedded files (possible compressed) into the binary.

struct File {
    uint8_t const* contents;
    size_t         compressed_sz;
    size_t         uncompressed_sz;
    bool           is_compressed;
    std::string    etag {};

    static File uncompressed(uint8_t const* contents, size_t sz, std::string const& etag={}) { return { contents, 0, sz, false, etag }; }
    static File compressed(uint8_t const* contents, size_t sz, size_t usz, std::string const& etag={}) { return { contents, sz, usz, true, etag }; }
};

using FileSet = std::unordered_map<fs::path, File>;

void deploy_file(File const& file, fs::path const& path);
void deploy_fileset(FileSet const& fileset, fs::path const& directory);

#endif //FILESET_HH
