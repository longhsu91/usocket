#include <uns_error.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <errno.h>
#include <string.h>

#define MAXLINE 1024

static void uns_error_internal(int is_errno, const char *format, va_list ap);

void uns_error_exit(const char *format, ...)
{
    va_list ap;

    va_start(ap, format);
    uns_error_internal(1, format, ap);
    va_end(ap);

    exit(1);
}

void uns_print(const char *format, ...)
{
    va_list ap;

    va_start(ap, format);
    uns_error_internal(0, format, ap);
    va_end(ap);
}

static void uns_error_internal(int is_errno, const char *format, va_list ap)
{
    char buf[MAXLINE + 1];

    vsprintf(buf, format, ap);
    int n = strlen(buf);
    if (is_errno)
    {
        snprintf(buf + n, MAXLINE - n, ":%s", strerror(errno));
    }
    printf("%s.\n", buf);

    return;
}