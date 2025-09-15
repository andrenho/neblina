#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <miniz.h>

int main(int argc, char* argv[])
{
    // parse arguments
    if (argc != 3) {
        fprintf(stderr, "Usage: %s INPUTFILE BASENAME", argv[0]);
        exit(EXIT_FAILURE);
    }

    const char* inputfile = argv[1];
    const char* basename = argv[2];

    // open file and get the size
    FILE* f = fopen(inputfile, "rb");
    fseek(f, 0, SEEK_END);
    long fsize = ftell(f);
    fseek(f, 0, SEEK_SET);

    // read contents
    char* contents = calloc(1, fsize + 1);
    fread(contents, fsize, 1, f);
    fclose(f);

    // Allocate buffers to hold compressed and uncompressed data.
    mz_ulong compressed_len = compressBound(fsize);
    uint8_t* compressed = malloc(compressed_len);

    // Compress the string.
    int cmp_status = compress(compressed, &compressed_len, (const unsigned char *) contents, fsize);
    if (cmp_status != Z_OK) {
        fprintf(stderr, "compress() failed!\n");
        return EXIT_FAILURE;
    }

    // output the compressed string
    printf("#include <stdint.h>\n");
    printf("#include <stddef.h>\n\n");
    printf("static size_t %s[] = {\n", basename);
    for (size_t i = 0; i < compressed_len; ++i) {
        if (i % 16 == 0) printf("    ");
        printf("0x%02x, ", compressed[i]);
        if (i % 16 == 15) printf("\n");
    }
    printf("\n};\n\n");
    printf("static size_t %s_compressed_sz = %zu;\n", basename, compressed_len);
    printf("static size_t %s_uncompressed_sz = %zu;\n", basename, fsize);

}
