#ifndef GZ_HH
#define GZ_HH

#include <stdint.h>
#include <stddef.h>

uint8_t const* gzip(uint8_t const* data, size_t usz, size_t* csz);
uint8_t const* gunzip(uint8_t const* data, size_t usz);

#endif //GZ_HH
