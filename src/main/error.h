#ifndef ERROR_H_
#define ERROR_H_

#define NON_RECOVERABLE_ERROR 57

void err_non_recoverable(const char* fmt, ...) __attribute__ ((format (printf, 1, 2)));
void err(const char* fmt, ...) __attribute__ ((format (printf, 1, 2)));

#endif
