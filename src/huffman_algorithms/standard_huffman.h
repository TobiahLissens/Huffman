//
// Created by tobiah on 03/10/17.
//

#ifndef HUFFMAN_PROJECT_STANDARD_HUFFMAN_H
#define HUFFMAN_PROJECT_STANDARD_HUFFMAN_H

#include "../util/io_operations.h"

/**
 * encodes bytes from input to output following standard huffman encoding
 * @param input pointer to huffman_stream for input
 * @param output pointer to huffman_stream for output
 */
void encode_standard_huffman(huffman_stream* input, huffman_stream* output);

/**
 * decodes bytes from input to output following standard huffman encoding
 * @param input pointer to huffman_stream for input
 * @param output pointer to huffman_stream for output
 */
void decode_standard_huffman(huffman_stream* input, huffman_stream* output);                                                                                           //:)


#endif //HUFFMAN_PROJECT_STANDARD_HUFFMAN_H
