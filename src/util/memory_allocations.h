//
// Created by tobiah on 05/10/17.
//

#ifndef HUFFMAN_PROJECT_MEMORY_ALLOCATIONS_H
#define HUFFMAN_PROJECT_MEMORY_ALLOCATIONS_H

#include "errors.h"
#include <stdlib.h>

#define SAFE_REALLOC(ptr, size) safe_realloc(ptr, size, __func__)
#define SAFE_MALLOC(size) safe_malloc(size, __func__)
#define SAFE_CALLOC(size) safe_calloc( size, __func__)

/**
 * personal realloc that throws an error when the realloc fails.
 * @param ptr pointer to be reallocated
 * @param size size to malloc
 * @return start of address to the newly reallocated block.
 */
static inline void* safe_realloc(void* ptr, size_t size, const char* func) {
    void* tmp = realloc(ptr, size);
    if (tmp == NULL) {
        free(ptr);
        throw_error(OUT_OF_MEMORY, func, "memory allocation failed.");
    }
    return tmp;
}

/**
 * personal malloc that throws an error when the malloc fails
 * @param size size to be allocated.
 * @return start of address to the newly allocated block
 */
static inline void* safe_malloc(size_t size, const char* func) {
    void* ptr = malloc(size);
    if (ptr == NULL) {
        throw_error(OUT_OF_MEMORY, func, "memory allocation failed.");
    }
    return ptr;
}

/**
 * personal calloc that throws an error when the calloc fails.
 * @param size size to be allocated.
 * @return start of address to the newly allocated block
 */
static inline void* safe_calloc(size_t size, const char* func) {
    void* ptr = calloc(size, 1);
    if (ptr == NULL) {
        throw_error(OUT_OF_MEMORY, func, "memory allocation failed.");
    }
    return ptr;
}

#endif //HUFFMAN_PROJECT_MEMORY_ALLOCATIONS_H
