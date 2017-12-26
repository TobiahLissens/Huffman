//
// Created by tobiah on 17/10/17.
//


#ifndef HUFFMAN_PROJECT_TEST_HUFFMAN_ALGORITHMS_H
#define HUFFMAN_PROJECT_TEST_HUFFMAN_ALGORITHMS_H


#include "../../src/util/io_operations.h"

typedef void (* huffman_function)(huffman_stream*, huffman_stream*);

char* test_all_huffman_algorithms();

char* test_standard_huffman();

char* test_adaptive_huffman();

char* test_sliding_window_adaptive_huffman();

char* test_two_pass_huffman();

char* test_blockwise_adaptive_huffman();

char* test_huffman_algorithm(huffman_function encode, huffman_function decode);

char* test_decode_robustness_standaard_huffman();

char* test_decode_robustness_adaptive_huffman();

char* test_decode_robustness_sliding_window_huffman();

char* test_decode_robustness_two_pass_huffman();

char* test_decode_robustness_blockwise_huffman();

char* test_decode_robustness_algorithm(huffman_function decode);

#endif //HUFFMAN_PROJECT_TEST_HUFFMAN_ALGORITHMS_H


