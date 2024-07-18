#include <stddef.h>

void *memcpy(void *dest_str, const void * src_str, size_t n)
{
    int i = 0;
    while (i != n)
    {
        *(char*)&dest_str[i] = *(char*)&src_str[i];
        i++;
    }
}

void *memset(void *str, int c, size_t n)
{
    int i = 0;
    while (i != n)
    {
        *(char*)&str[i] = c;
        i++;
    }
}

int puts(const char *str)
{
    return 0;
}

int printf_placeholder(const char *format, ...)
{
    return 5;
}

int fprintf_placeholder(void* dummy, const char *format, ...)
{
    return 5;
}

int strcmp (const char* str1, const char* str2)
{
    return 0;
}