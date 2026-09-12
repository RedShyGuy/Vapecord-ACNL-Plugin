#pragma once

#include <stdarg.h>

#ifdef __cplusplus
extern "C"
{
#endif

void* malloc(size_t size);
void free(void* ptr);

int strcmp(const char* str1, const char* str2);
void* memcpy(void* destination, const void* source, size_t num);

#ifdef __cplusplus
#define CHAR16_TYPE wchar_t
#else
#define CHAR16_TYPE __CHAR16_TYPE__
#endif

CHAR16_TYPE* wcsncpy(CHAR16_TYPE* destination, const CHAR16_TYPE* source, size_t num);

size_t  wcslen(const CHAR16_TYPE* wcs);
CHAR16_TYPE* wcscat(CHAR16_TYPE* destination, const CHAR16_TYPE* source);

int swprintf(CHAR16_TYPE* ws, size_t len, const CHAR16_TYPE* format, ...);
int	vswprintf (CHAR16_TYPE* __restrict, size_t, const CHAR16_TYPE* __restrict, va_list);

#ifdef __cplusplus
}
#endif
