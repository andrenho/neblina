#ifndef GZ_HH
#define GZ_HH

#include <stdint.h>
#include <stddef.h>

uint8_t const* gzip(uint8_t const* uncompressed, size_t usz, size_t* csz);   // returns NULL in case of errors
uint8_t const* gunzip(uint8_t const* compressed, size_t csz, size_t* usz);

#endif //GZ_HH
