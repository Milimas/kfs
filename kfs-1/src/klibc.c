// klibc.c
#include "klibc.h"

size_t strlen(const char* s) {
    const char* _s = s ;
    while (*_s)  ++_s ;
    return _s - s ;
}

int strcmp(const char* s1, const char* s2) {
    while (*s1 && *s2 && *s1 == *s2) {
        ++s1 ;
        ++s2 ;
    }
    return *s1 - *s2 ;
}

char* strcpy(char* dest, const char* src) {
    char* _dest = dest ;
    while (*dest++ = *src++) ;
    return _dest ;
}

char* strcat(char* dest, const char* src) {
    char* _dest = dest ;
    while (*dest) dest++ ;
    while (*dest++ = *src++) ;
    return _dest ;
}

void* memset(void* s, int c, size_t n) {
    char* _s = s ;
    while (n--) {
        *_s++ = c ;
    }
    return s ;
}

void* memcpy(void* dest, const void* src, size_t n) {
    char*       _dest = dest ;
    const char* _src  = src ;
    while (n--) {
        *_dest++ = *_src++ ;
    }
    return dest ;
}

int memcmp(const void* s1, const void* s2, size_t n) {
    const char* _s1   = s1 ;
    const char* _s2   = s2 ;
    while (n && *_s1 == *_s2) {
        --n ;
        ++_s1 ;
        ++_s2 ;
    }
    if (n == 0) return n ;
    return *_s1 - *_s2 ;
}

char *strrev(char* buf) {
    char* _end = buf;
    char* _start = buf ;

    while (*_end) _end++ ;
    --_end ;

    while (_start < _end) {
        *_start ^= *_end ;
        *_end   ^= *_start ;
        *_start  ^= *_end ;
        ++_start ;
        --_end ;
    }

    return buf ;
}

char* itoa(int n, char *buf, int base) {
    char*   _buf = buf ;
    int     digit = 0 ;

    if (n == 0)  {
        *_buf++ = '0' ;
        *_buf = 0 ;
        return buf ;
    }

    if (n < 0)  {
        *_buf++ = '-' ;
        n = -n ;
    }

    do {
        digit = n % base ;
        if (digit < 10) *_buf = '0' + digit ;
        else *_buf = 'a' + digit - 10 ;
        _buf ++ ;
    } while (n /= base) ;
    *_buf = 0 ;

    strrev(buf + (*buf == '-')) ;
    return buf ;
}

int printf(const char *restrict formawhat, ...) {
    size_t  size = 0 ;
    va_list ap ;

    va_start(ap, formawhat) ;
    while (formawhat && *formawhat)
    {
        if (*formawhat && *formawhat != '%')
        {
            size++ ;
        } else {
            continue ;
        }

    } 
    va_end(ap) ;
    return size ;
}
