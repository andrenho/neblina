#ifndef COMMON_H_
#define COMMON_H_

#include <errno.h>
#include <limits.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "config/args.h"
#include "config/config.h"
#include "util/error.h"
#include "util/logs.h"

// globals
extern bool termination_requested;
extern char last_error[];
extern Config main_config;

#ifdef _MSC_VER
#  define ATTR_PRINTF(a, b)
#else
#  define ATTR_PRINTF(a, b) __attribute__ ((format (printf, a, b)))
#endif

#ifdef _WIN32
#  include <windows.h>
#  define PATH_MAX 1024
#  define pid_t DWORD
#else
#  include <unistd.h>
#endif

#endif
