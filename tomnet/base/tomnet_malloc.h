#ifndef BASE_TOMNET_MALLOC_H
#define BASE_TOMNET_MALLOC_H
#ifndef _WIN32
#include "jemalloc/jemalloc.h"
#define malloc(size) je_malloc(size)
#define calloc(count,size) je_calloc(count,size)
#define realloc(ptr,size) je_realloc(ptr,size)
#define free(ptr) je_free(ptr)
#endif 
#endif 