//
// Created by tobiah on 03/10/17.
//

#ifndef HUFFMAN_PROJECT_BLOCKWISE_ADAPTIVE_HUFFMAN_H
#define HUFFMAN_PROJECT_BLOCKWISE_ADAPTIVE_HUFFMAN_H

#include "../util/io_operations.h"
#include "../util/settings.h"
/**
 * Block size size in bytes
 */
#define BLOCK_SIZE (size == 0 ? 32768: size) //2^15

/**
 * encodes bytes from input to output following blockwise adaptive huffman encoding
 * @param input pointer to huffman_stream for input
 * @param output pointer to huffman_stream for output
 */
void encode_blockwise_adaptive_huffman(huffman_stream* input, huffman_stream* output);

/**
 * decodes bytes from input to output following blockwise adaptive huffman encoding
 * @param input pointer to huffman_stream for input
 * @param output pointer to huffman_stream for output
 */
void decode_blockwise_adaptive_huffman(huffman_stream* input,
                                       huffman_stream* output);                                                                                           //:)

#endif //HUFFMAN_PROJECT_BLOCKWISE_ADAPTIVE_HUFFMAN_H
