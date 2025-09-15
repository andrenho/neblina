#ifndef EXAMPLE_HH
#define EXAMPLE_HH

#include <cstdint>
#include <string>
#include <unordered_map>

struct File {
    uint8_t const* contents;
    size_t         compressed_sz;
    size_t         uncompressed_sz;
    bool           is_compressed;

    static File uncompressed(uint8_t const* contents, size_t sz) { return { contents, 0, sz, false }; }
    static File compressed(uint8_t const* contents, size_t sz, size_t usz) { return { contents, usz, sz, true }; }
};

using FileSet = std::unordered_map<std::string, File>;

inline const FileSet files = {
    { "/index.html", File::compressed((uint8_t const[]) { 0x12, 0x24 }, 1, 2) },
};

#endif //EXAMPLE_HH
