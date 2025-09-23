#ifndef ERROR_H_
#define ERROR_H_

#define NON_RECOVERABLE_ERROR 57

#ifndef _MSC_VER
#  define ATTR_PRINTF(a, b) __attribute__ ((format (printf, a, b)))
#else
#  define ATTR_PRINTF(a, b)
#endif

extern char last_error[];

void err_non_recoverable(const char* fmt, ...) ATTR_PRINTF(1, 2);
void err_fatal(const char* fmt, ...) ATTR_PRINTF(1, 2);
void err(const char* fmt, ...) ATTR_PRINTF(1, 2);

#define THROW_NULL(...) { err(__VA_ARGS__); return NULL; }
#define THROW(...) { err(__VA_ARGS__); return false; }

#endif
