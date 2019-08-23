#ifndef _UNS_ERROR_H_INCLUDE_
#define _UNS_ERROR_H_INCLUDE_

#define MAXLINE 1024

void uns_print(const char *format, ...);

void uns_error_exit(const char *format, ...);

#endif