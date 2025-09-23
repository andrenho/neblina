#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <dirent.h>

#include "../../src/file/gz.h"
#include "../../src/file/whole_file.h"

#define MAX_FILENAME 8128
#define MAX_FILES 256

const char* compressable_extensions[] = { 
    ".txt", ".html", ".js", ".css", ".json", ".bmp", ".csv", ".mid", ".midi",
    NULL
};

const char *basename_from_path(const char *path) {
    if (!path || !*path) return path;
   
    const char *slash1 = strrchr(path, '/');  // UNIX separator
    const char *slash2 = strrchr(path, '\\'); // Windows separator

    const char *last_sep = slash1 > slash2 ? slash1 : slash2;
    return last_sep ? last_sep + 1 : path;
}

void remove_extension(char *filename) {
    if (!filename) return;

    char *last_dot = strrchr(filename, '.');
    char *last_slash1 = strrchr(filename, '/');
    char *last_slash2 = strrchr(filename, '\\');
    char *last_slash = last_slash1 > last_slash2 ? last_slash1 : last_slash2;

    if (last_dot && (!last_slash || last_dot > last_slash)) {
        *last_dot = '\0';
    }
}

bool is_compressable(const char* extension)
{
    size_t i = 0;
    while (compressable_extensions[i]) {
        if (strcmp(compressable_extensions[i], extension) == 0)
            return true;
        ++i;
    }
    return false;
}

static void generate_file(const char* path, char* out_name)
{
    const char* basename = basename_from_path(path);
    const char* extension = strrchr(path, '.');
    strcpy(out_name, basename);
    remove_extension(out_name);

    char* n = strdup(out_name);
    sprintf(out_name, "%s_%s", n, &extension[1]);
    free(n);

    size_t file_sz;
    size_t cmp_sz = 0;
    uint8_t* data = whole_file_read(path, &file_sz);
    if (is_compressable(extension)) {
        uint8_t* compressed = gzip(data, file_sz, &cmp_sz);
        free(data);
        data = compressed;
    }

    printf("static NFile %s = {\n", out_name);
	printf("    .name = \"%s\"\n", basename);
	printf("    .contents = (uint8_t const[]) {\n");
    for (size_t i = 0; i < file_sz; ++i) {
        if ((i % 16) == 0) printf("        ");
        printf("0x%02X, ", data[i]);
        if ((i % 16) == 15) printf("\n");
    }
    printf("\n    },\n");
	printf("    .compressed_sz = %zu\n", cmp_sz);
	printf("    .uncompressed_sz = %zu\n", file_sz);
    printf("};\n\n");

    free(data);
}

static size_t generate_dir(const char* path, char** files)
{
    size_t count = 0;

    DIR* dir = opendir(path);
    if (!dir) abort();

    struct dirent* dp;
    while ((dp = readdir(dir)) != NULL) {
        if (strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0)
            continue;

        char fpath[1024];
        snprintf(fpath, sizeof(fpath), "%s/%s", path, dp->d_name);
        
        char* outname = calloc(1, MAX_FILENAME);
        generate_file(fpath, outname);
        files[count] = outname;

        ++count;
    }

    closedir(dir);

    return count;
}

int main(int argc, char* argv[])
{
    const char* path;
    bool        is_dir = false;

    if (argc == 2 && strcmp(argv[1], "-d") != 0) {
        path = argv[1];
    } else if (argc == 3 && strcmp(argv[1], "-d") == 0) {
        is_dir = true;
        path = argv[2];
    } else {
        fprintf(stderr, "Usage: %s [-d] PATH\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    printf("#include \"file/fileset.h\"\n\n");

    char out_name[MAX_FILENAME];
    if (!is_dir) {
        generate_file(path, out_name);
    } else {
        char* files[MAX_FILES] = { NULL };
        size_t n_files = generate_dir(path, files);
        printf("static NFileSet fileset = {\n");
        printf("    .n_files = %zu\n", n_files);
        printf("    .files = (NFile const*[]) { ");
        for (size_t i = 0; i < n_files; ++i) {
            printf("&%s, ", files[i]);
            free(files[i]);
        }
        printf("},\n};\n");
    }
}
