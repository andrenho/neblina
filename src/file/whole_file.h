#ifndef WHOLE_FILE_HH
#define WHOLE_FILE_HH

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

uint8_t* whole_file_read(const char* path, size_t* sz_out);
bool     whole_file_write(const char* path, uint8_t const* data, size_t sz);

#endif //WHOLE_FILE_HH
