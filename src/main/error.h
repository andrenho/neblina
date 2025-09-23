#ifndef ERROR_H_
#define ERROR_H_

#define NON_RECOVERABLE_ERROR 57

#ifndef _MSC_VER
#  define ATTR_PRINTF(a, b) __attribute__ ((format (printf, a, b)))
#else
#  define ATTR_PRINTF(a, b)
#endif

void err_non_recoverable(const char* fmt, ...) ATTR_PRINTF(1, 2);
void err(const char* fmt, ...) ATTR_PRINTF(1, 2);

#endif
