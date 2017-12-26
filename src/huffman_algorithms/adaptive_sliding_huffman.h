//
// Created by tobiah on 03/10/17.
//

#ifndef HUFFMAN_PROJECT_ADAPTIVE_SLIDING_HUFFMAN_H
#define HUFFMAN_PROJECT_ADAPTIVE_SLIDING_HUFFMAN_H

#include "../util/io_operations.h"
#include "../util/settings.h"
/**
 * sliding window byte size
 */
#define SLIDING_WINDOW_SIZE ((size == 0) ? 4096 : size)


/**
 * encodes bytes from input to output following adaptive sliding huffman encoding
 * @param input pointer to huffman_stream for input
 * @param output pointer to huffman_stream for output
 */
void encode_adaptive_sliding_huffman(huffman_stream* input, huffman_stream* output);


/**
 * decodes bytes from input to output following adaptive sliding huffman decoding
 * @param input pointer to huffman_stream for input
 * @param output pointer to huffman_stream for output
 */
void decode_adaptive_sliding_huffman(huffman_stream* input, huffman_stream* output);

#endif //HUFFMAN_PROJECT_ADAPTIVE_SLIDING_HUFFMAN_H
