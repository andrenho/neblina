#include "os/window.h"

#include "common.h"

#define ESC "\x1B"

void vnprintf(const char *restrict fmt, va_list ap)
{
    // TODO
    vfprintf(stdout, fmt, ap);
}

void vnprintf_error(const char *restrict fmt, va_list ap)
{
    // TODO
    vfprintf(stderr, fmt, ap);
}

void window_init()
{
}

void window_close()
{
}
