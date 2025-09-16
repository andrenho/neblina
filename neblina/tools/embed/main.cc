#include <assert.h>
#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <cctype>

#include <filesystem>
#include <iostream>
#include <string>
#include <vector>
namespace fs = std::filesystem;

#include "../../file/gz.hh"
#include "../../file/whole_file.hh"

static std::vector<std::string> compressable_extensions = {
    ".txt", ".html", ".js", ".css", ".json", ".bmp", ".csv", ".mid", ".midi"
};

static void generate_file(std::string const& filename)
{
    std::string extension = fs::path(filename).extension();
    auto contents = whole_file::read_bytes(filename);

    bool compress = std::ranges::find(compressable_extensions, extension) != compressable_extensions.end();
    if (contents.size() < 60)
        compress = false;

    auto cout_binary = [](std::vector<uint8_t> const& contents) {
        for (uint8_t v: contents) std::cout << std::format("{:#x}, ", v);
    };

    if (compress) {
        auto compressed = gz::gzip(contents);
        std::cout << "File::compressed((uint8_t const[]) { ";
        cout_binary(compressed);
        std::cout << std::format("}}, {}, {})", compressed.size(), contents.size());

        // test decompression
        assert(gz::gunzip(compressed) == contents);
    } else {
        std::cout << "File::uncompressed((uint8_t const[]) { ";
        cout_binary(contents);
        std::cout << std::format("}}, {})", contents.size());
    }
}

int main(int argc, char* argv[])
{
    // parse arguments
    if (argc != 3 && argc != 4) {
        fprintf(stderr, "Usage: %s [-d] INPUTFILE BASENAME", argv[0]);
        exit(EXIT_FAILURE);
    }

    bool directory;
    std::string inputfile, basename;
    if (std::string(argv[1]) == "-d") {
        directory = true;
        inputfile = argv[2];
        basename = argv[3];
    } else {
        directory = false;
        inputfile = argv[1];
        basename = argv[2];
    }

    std::string ubase = basename;
    std::transform(ubase.begin(), ubase.end(), ubase.begin(), ::toupper);

    std::cout << std::format(R"(
#ifndef {}_HH
#define {}_HH

#include "file/fileset.hh"

)", ubase, ubase);

    if (directory) {
        std::cout << std::format("inline const FileSet {} = {{\n", basename);
        fs::path dir(inputfile);
        for (auto const& entry: fs::recursive_directory_iterator(dir)) {
            if (entry.is_regular_file() && !entry.path().filename().string().starts_with(".")) {
                std::cout << std::format("    {{ \"{}\", ", fs::relative(entry.path(), dir).string());
                generate_file(entry.path());
                std::cout << "},\n";
            }
        }
        std::cout << "};\n";
    } else {
        std::cout << std::format("inline const File {} = ", basename);
        generate_file(inputfile);
        std::cout << ";\n";
    }

    std::cout << std::format("\n#endif // {}_HH\n", basename);
}
