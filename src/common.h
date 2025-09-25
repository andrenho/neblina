#ifndef COMMON_H_
#define COMMON_H_

#include <errno.h>
#include <limits.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "main/args.h"
#include "util/logs.h"
#include "main/error.h"
#include "config/config.gen.h"

// globals
extern bool termination_requested;
extern char last_error[];
extern struct Config main_config;

#ifndef _MSC_VER
#  define ATTR_PRINTF(a, b) __attribute__ ((format (printf, a, b)))
#else
#  define ATTR_PRINTF(a, b)
#endif

#endif
