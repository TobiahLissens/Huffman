//
// Created by tobiah on 04.11.17.
//

#ifndef HUFFMAN_PROJECT_TEST_IO_OPERATIONS_H
#define HUFFMAN_PROJECT_TEST_IO_OPERATIONS_H

#include "../../src/util/io_operations.h"

void test_all_io_operations();

char* huffman_stream_rewind_test();

char* huffman_stream_read_byte_test();

char* huffman_stream_read_bit_test();

char* huffman_stream_read8_bit_test();

char* huffman_stream_read_int_test();

char* huffman_stream_write_byte_test();

char* huffman_stream_write_bit_test();

char* huffman_stream_write_8bit_test();

char* path_cache_fwrite_test();

char* huffman_stream_padd_bits_test();

#endif //HUFFMAN_PROJECT_TEST_IO_OPERATIONS_H
