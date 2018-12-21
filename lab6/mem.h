#ifndef _MEM_H_
#define _MEM_H_

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/mman.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#define HEAP_START ((void*)0x04040000)
#define DEBUG_FIRST_BYTES 32
#define BLOCK_MIN_SIZE 32


#pragma pack(push, 1)
typedef struct{
    struct mem *next;
    size_t capacity;
    bool is_free;
} mem;
#pragma pack(pop)

void* _malloc(size_t query);
void _free(void *mem);
void* heap_init(size_t initial_size);

#endif