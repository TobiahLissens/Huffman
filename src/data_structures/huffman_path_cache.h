//
// Created by tobiah on 18.10.17.
//

#ifndef HUFFMAN_PROJECT_STANDARD_HUFFMAN_PATH_CACHE_H
#define HUFFMAN_PROJECT_STANDARD_HUFFMAN_PATH_CACHE_H

#include "../util/io_operations.h"
#include "stddef.h"
#include <stdbool.h>
#include <stdint.h>
#include <memory.h>

typedef struct path_cache {
    /**
     * Important note:
     * In theory the depth of a tree and as a consequence the path to a leaf can be 255 long.
     * But in practice the path will never exceed 64.
     * The worst case is a tree with one leaf at every level.
     * This tree can only be created when the frequency table follows the fibonacci sequence
     * if we calculate the size of a file following the fibonacci sequence we see that a tree with depth > 64
     * will be a few terra bytes long and this obviously isn't a real world use case(for this project)
     */
    uint64_t line;

    //length of the path
    uint16_t length;

    //length of the cache already seen (only used in the decoding process)
    uint16_t seen;
} path_cache;

typedef path_cache* path_cache_block;

/**
 * initializes a path cache_block
 * @param cache_block list of cache_paths to initialize
 * @param size length of the patch_cache_block
 */
static inline void cache_block_init(path_cache_block cache_block, uint16_t size) {
    memset(cache_block, 0, size * sizeof(*cache_block));
}

/**
 * Initialize a single path cache
 * @param cache pointer to the path_cache to initialize
 * @param length length of the path
 */
static inline void path_cache_init(path_cache* cache, uint16_t length) {
    cache->line = 0;
    cache->length = length;
    cache->seen = length;
}

/**
 * write out a path_cache to an stream
 * @param stream pointer to the stream
 * @param cache cache to write to the stream
 */
static inline void path_cache_fwrite(huffman_stream* stream, path_cache* cache) {
    NULL_CHECK(stream);

    huffman_stream_write_n_bits(stream, cache->line, cache->length);
}

#endif //HUFFMAN_PROJECT_STANDARD_HUFFMAN_PATH_CACHE_H
