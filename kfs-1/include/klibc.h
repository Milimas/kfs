// klibc.h
#ifndef KLIB_H
#define KLIB_H

#include <stdint.h>
#include <stdarg.h>

#define size_t  uint32_t
#define NULL    (void*)0

size_t strlen(const char* s) ;
int strcmp(const char* s1, const char* s2) ;
char* strcpy(char* dest, const char* src) ;
char* strcat(char* dest, const char* src) ;
void* memset(void* s, int c, size_t n) ;
void* memcpy(void* dest, const void* src, size_t n) ;
int memcmp(const void* s1, const void* s2, size_t n) ;
char* itoa(int n, char *buf, int base) ;
char* strrev(char* s) ;


#define KERN_INFO   "[INFO]"
#define KERN_ERR    "[ERROR]"

int printf(const char *restrict formawhat, ...) ;
int printk(const char *restrict formawhat, ...) ;

#endif
