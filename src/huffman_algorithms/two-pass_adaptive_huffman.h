//
// Created by tobiah on 03/10/17.
//

#ifndef HUFFMAN_PROJECT_TWO_PASS_ADAPTIVE_HUFFMAN_H
#define HUFFMAN_PROJECT_TWO_PASS_ADAPTIVE_HUFFMAN_H

#include "../util/io_operations.h"

/**
 * encodes bytes from input to output following two-pass adaptive huffman encoding
 * @param input pointer to huffman_stream for input
 * @param output pointer to huffman_stream for output
 */
void encode_two_pass_adaptive_huffman(huffman_stream* input, huffman_stream* output);


/**
 * decodes bytes from input to output following two-pass adaptive huffman encoding
 * @param input pointer to huffman_stream for input
 * @param output pointer to huffman_stream for output
 */
void decode_two_pass_adaptive_huffman(huffman_stream* input,
                                      huffman_stream* output);                                                                                           //:)

#endif //HUFFMAN_PROJECT_TWO_PASS_ADAPTIVE_HUFFMAN_H
