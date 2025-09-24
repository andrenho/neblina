#ifndef NEBLINA_WINDOW_H
#define NEBLINA_WINDOW_H

#include <stdarg.h>

void vnprintf(const char *restrict fmt, va_list ap);
void vnprintf_error(const char *restrict fmt, va_list ap);

void nprintf(const char *restrict fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    vnprintf(fmt, ap);
    va_end(ap);
}

void nprintf_error(const char *restrict fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    vnprintf_error(fmt, ap);
    va_end(ap);
}

#endif //NEBLINA_WINDOW_H
