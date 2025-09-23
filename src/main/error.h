#ifndef ERROR_H_
#define ERROR_H_

#define NON_RECOVERABLE_ERROR 57

void err_non_recoverable(const char* fmt, ...);
void err(const char* fmt, ...);

#endif
