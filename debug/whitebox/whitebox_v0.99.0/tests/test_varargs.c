// varargs functions
#include <stdarg.h>

int varargs(int n, ...)
{
    int result = 0;
    va_list args;
    va_start(args, n);

    for (int i = 0; i < n; ++i)
    {
        int x = va_arg(args, int);
        result += x;
    }
    va_end(args);

    return result;
}

int varargsv(int n, va_list args)
{
    int result = 0;

    for (int i = 0; i < n; ++i)
    {
        int x = va_arg(args, int);
        result += x;
    }

    return result;
}

int varargsv_call(int n, ...)
{
    va_list args;
    va_start(args, n);
    int result = varargsv(n, args);
    va_end(args);
    return result;
}

#include <stdio.h>
int custom_sprintf(char *buffer, int size, char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    int result = vsnprintf(buffer, size, fmt, args);
    va_end(args);
    return result;
}

int simple_string(char *in)
{
    return !!in;
}
