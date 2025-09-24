#ifndef ERROR_H_
#define ERROR_H_

#include "common.h"
#include "util/logs.h"

#define NON_RECOVERABLE_ERROR 57

void fatal_non_recoverable(const char* fmt, ...) ATTR_PRINTF(1, 2);
void fatal(const char* fmt, ...) ATTR_PRINTF(1, 2);

#define THROW_NULL(...) { ERR(__VA_ARGS__); return NULL; }
#define THROW(...) { ERR(__VA_ARGS__); return false; }
#define THROW_PROPAGATE { return false; }
#define THROW_ERRNO { ERR("%s", strerror(errno)); return false; }

#endif
