//
// Created by tobiah on 02.11.17.
//

#ifndef HUFFMAN_PROJECT_FILE_CACHE_H
#define HUFFMAN_PROJECT_FILE_CACHE_H

#include "io_operations.h"
#include "memory_allocations.h"
#include <stdint.h>

#define BUFFER_INITIAL (1048576)
#define BUFFER_MAX (BUFFER_INITIAL * 1024)
#define RESIZE_FACTOR 2

typedef struct file_cache {

    //file buffer
    uint8_t* buffer;

    //size of the buffer
    size_t size;

    //current capacity
    size_t capacity;
} file_cache;

/**
 * read a file into main memory
 * @param cache the
 * @param stream stream to read the file from
 */
static inline void file_cache_init(file_cache* cache, huffman_stream* stream) {
    cache->capacity = BUFFER_INITIAL;
    cache->size = 0;
    cache->buffer = SAFE_MALLOC(sizeof(uint8_t) * BUFFER_INITIAL);
    size_t read;

    while ((read = fread(cache->buffer + cache->size,
                         sizeof(uint8_t), cache->capacity - cache->size, stream->file_stream)) == cache->capacity - cache->size) {
        cache->size += read;
        if (cache->size == cache->capacity) {
            cache->capacity = (cache->capacity <= BUFFER_MAX) ? cache->capacity * RESIZE_FACTOR : cache->capacity +
                                                                                                  BUFFER_MAX;
            cache->buffer = SAFE_REALLOC(cache->buffer, cache->capacity);
        }
    }

    //add the last bytes read
    cache->size += read;
}

/**
 * get the size of the file_cache
 * @param cache pointer to the file cache
 * @return size of the cache
 */
static inline size_t file_cache_size(file_cache* cache) {
    return cache->size;
}

/**
 * get a byte from the file cache
 * @param cache pointer to the file_cache
 * @param index the requested index of the file cache
 * @return byte at the specified index in the file cache
 */
static inline int file_cache_get(file_cache* cache, size_t index) {
    return cache->buffer[index];
}

/**
 * free the cache
 * @param cache pointer to a cache
 */
static inline void file_cache_free(file_cache* cache) {
    free(cache->buffer);
}

/**
 * count the frequencies of bytes in the file cache
 * @param cache pointer to the file cache
 * @param frequency_map array used as frequency map
 */

static inline void count_frequencies(file_cache cache, uint64_t* frequency_map) {
    for (size_t i = 0; i < file_cache_size(&cache); ++i) {
        frequency_map[file_cache_get(&cache, i)]++;
    }
}
#endif //HUFFMAN_PROJECT_FILE_CACHE_H
