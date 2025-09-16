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

    } else {
        std::cout << std::format("inline const File {} = ", basename);
        generate_file(inputfile);
        std::cout << ";\n";
    }

    std::cout << std::format("\n#endif // {}_HH\n", basename);

    /*
    // open file and get the size
    FILE* f = fopen(inputfile, "rb");
    fseek(f, 0, SEEK_END);
    long fsize = ftell(f);
    fseek(f, 0, SEEK_SET);

    // read contents
    char* contents = (char *) calloc(1, fsize + 1);
    fread(contents, fsize, 1, f);
    fclose(f);

    // Allocate buffers to hold compressed and uncompressed data.
    mz_ulong compressed_len = compressBound(fsize);
    uint8_t* compressed = (uint8_t *) malloc(compressed_len);

    // Compress the string.
    int cmp_status = compress(compressed, &compressed_len, (const unsigned char *) contents, fsize);
    if (cmp_status != Z_OK) {
        fprintf(stderr, "compress() failed!\n");
        return EXIT_FAILURE;
    }

    // output the compressed string
    printf("#include <stdint.h>\n");
    printf("#include <stddef.h>\n\n");
    printf("static uint8_t const %s[] = {\n", basename);
    for (size_t i = 0; i < compressed_len; ++i) {
        if (i % 16 == 0) printf("    ");
        printf("0x%02x, ", compressed[i]);
        if (i % 16 == 15) printf("\n");
    }
    printf("\n};\n\n");
    printf("static size_t %s_compressed_sz = %zu;\n", basename, compressed_len);
    printf("static size_t %s_uncompressed_sz = %zu;\n", basename, fsize);
    */
}
