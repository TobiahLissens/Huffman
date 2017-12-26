//
// Created by tobiah on 04.11.17.
//

#include <string.h>
#include "test_io_operations.h"

#define test_strings_size 4

static huffman_stream stream;

static char* test_strings[] = {"hello_world", "0123456789", "", "aaaaaaaaa"};

huffman_stream* create_test_stream(char* string) {
    huffman_stream_init(fmemopen(string, strlen(string) + 1, "r+"), &stream);
    return &stream;
}

huffman_stream* create_empty_test_stream() {
    huffman_stream_init(tmpfile(), &stream);
    return &stream;
}

void test_all_io_operations() {
    test(huffman_stream_rewind_test);
    test(huffman_stream_read_byte_test);
    test(huffman_stream_read_bit_test);
    test(huffman_stream_read8_bit_test);
    test(huffman_stream_read_int_test);
    test(huffman_stream_write_byte_test);
    test(huffman_stream_write_bit_test);
    test(huffman_stream_write_8bit_test);
    test(path_cache_fwrite_test);
    test(huffman_stream_padd_bits_test);
}

char* huffman_stream_rewind_test() {
    huffman_stream* huffman_stream = create_test_stream(test_strings[0]);
    int first = huffman_stream_read_byte(huffman_stream);
    huffman_stream_rewind(huffman_stream);
    int first_2 = huffman_stream_read_byte(huffman_stream);
    assert_true(first == first_2);
    return NULL;
}

char* huffman_stream_read_byte_test() {
    for (int i = 0; i < test_strings_size; ++i) {
        huffman_stream* pStream = create_test_stream(test_strings[i]);
        int c;
        int index = 0;
        while ((c = huffman_stream_read_byte(pStream)) != EOF) {
            assert_true(c == test_strings[i][index]);
            index++;
        }
    }
    return NULL;
}

char* huffman_stream_read_bit_test() {
    for (int i = 0; i < test_strings_size; ++i) {
        huffman_stream* pStream = create_test_stream(test_strings[i]);
        int c;
        size_t index = 0;
        size_t sub_index = 0;
        while ((c = huffman_stream_read_bit(pStream)) != EOF) {
            int bit = ((test_strings[i][index] >> (sizeof(char) * CHAR_BIT - 1 - sub_index))) & 1;
            assert_true(c == bit);
            sub_index++;
            if (sub_index >= 8) {
                sub_index = 0;
                index++;
            }
        }
    }
    return NULL;
}

char* huffman_stream_read8_bit_test() {
    for (int i = 0; i < test_strings_size; ++i) {
        huffman_stream* pStream = create_test_stream(test_strings[i]);
        uint64_t c = 0;
        int index = 0;
        while ((huffman_stream_read_n_bits(pStream, &c, BYTE_SIZE)) == BYTE_SIZE) {
            assert_true(c == test_strings[i][index]);
            index++;
        }
    }
    return NULL;
}

char* huffman_stream_read_int_test() {
    int test = 0x00010203;

    huffman_stream* pStream = create_empty_test_stream();

    for (int i = 0; i < sizeof(uint32_t); ++i) {
        huffman_stream_write_byte(pStream, i);
    }


    uint64_t c;
    int index = 0;
    huffman_stream_flush(pStream);
    huffman_stream_rewind(pStream);

    while ((huffman_stream_read_n_bits(pStream, &c, 32)) == 32) {
        assert_true(c == test);
        index++;
    }

    huffman_stream_flush(pStream);
    huffman_stream_rewind(pStream);

    for (int j = 0; j < 8000; ++j) {
        huffman_stream_write_n_bits(pStream, (uint32_t) j, 32);
    }

    huffman_stream_pad_bits(pStream, PAD_N_ZERO);
    huffman_stream_flush(pStream);
    huffman_stream_rewind(pStream);

    for (int k = 0; k < 8000; ++k) {
        uint64_t i = 0;
        huffman_stream_read_n_bits(pStream, &i, 32);
        //int i = huffman_stream_read_uint32(pStream);
        assert_true(i == k);
    }


    return NULL;
}

char* huffman_stream_write_byte_test() {
    for (int i = 0; i < test_strings_size; ++i) {
        huffman_stream* pStream = create_empty_test_stream();
        char* current_string = test_strings[i];
        for (int k = 0; k < strlen(current_string) + 1; ++k) {
            huffman_stream_write_byte(pStream, current_string[k]);
        }
        huffman_stream_flush(pStream);
        huffman_stream_rewind(pStream);
        int c;
        int index = 0;
        while ((c = getc(pStream->file_stream)) != EOF) {
            assert_true(c == test_strings[i][index]);
            index++;
        }
    }
    return NULL;
}

char* huffman_stream_write_bit_test() {
    for (int i = 0; i < test_strings_size; ++i) {
        huffman_stream* pStream = create_empty_test_stream();
        char* current_string = test_strings[i];
        for (int k = 0; k < strlen(current_string) + 1; ++k) {
            for (int j = 0; j < 8; ++j) {
                int bit = (current_string[k] >> (sizeof(char) * CHAR_BIT - 1 - j)) & 1;
                huffman_stream_write_bit(pStream, bit);
            }
        }
        huffman_stream_flush(pStream);
        huffman_stream_rewind(pStream);
        int c;
        int index = 0;
        while ((c = getc(pStream->file_stream)) != EOF) {
            assert_true(c == test_strings[i][index]);
            index++;
        }
    }
    return NULL;
}

char* huffman_stream_write_8bit_test() {
    for (int i = 0; i < test_strings_size; ++i) {
        huffman_stream* pStream = create_empty_test_stream();
        char* current_string = test_strings[i];
        for (int k = 0; k < strlen(current_string) + 1; ++k) {
            huffman_stream_write_byte(pStream, current_string[k]);
        }
        huffman_stream_flush(pStream);
        huffman_stream_rewind(pStream);
        int c;
        int index = 0;
        while ((c = getc(pStream->file_stream)) != EOF) {
            assert_true(c == test_strings[i][index]);
            index++;
        }
    }
    return NULL;
}

char* path_cache_fwrite_test() {
    return NULL;
}

char* huffman_stream_padd_bits_test() {
    char bit_sequence[10][8] = {
            {},
            {1},
            {0, 1},
            {0, 1, 1},
            {0, 0, 1, 1},
            {0, 1, 0, 1, 0},
            {1, 1, 1, 1, 1, 0},
            {1, 1, 0, 0, 0, 1, 1},
            {1, 1, 1, 1, 1, 1, 1, 1},
    };
    //char length[] = {1, 2, 3, 4, 5, 6, 7, 8};
    int solution1[] = {-1, 128, 64, 96, 48, 80, 248, 198, 255};
    int solution2[][2] = {{-1, -1},
                          {1,  128},
                          {2,  64},
                          {3,  96},
                          {4,  48},
                          {5,  80},
                          {6,  248},
                          {7,  198},
                          {8,  255}};

    for (int i = 0; i < 9; ++i) {
        huffman_stream* pStream = create_empty_test_stream();
        for (int j = 0; j < i; ++j) {
            huffman_stream_write_bit(pStream, bit_sequence[i][j]);
        }
        huffman_stream_pad_bits(pStream, PAD);
        huffman_stream_flush(pStream);
        huffman_stream_rewind(pStream);

        int byte = huffman_stream_read_byte(pStream);
        //printf("%d\n", byte);
        assert_true(solution1[i] == byte);
    }

    for (int i = 0; i < 9; ++i) {
        huffman_stream* pStream = create_empty_test_stream();
        for (int j = 0; j < i; ++j) {
            huffman_stream_write_bit(pStream, bit_sequence[i][j]);
        }
        huffman_stream_pad_bits(pStream, PAD_N_ZERO);
        huffman_stream_flush(pStream);
        huffman_stream_rewind(pStream);

        int byte = huffman_stream_read_byte(pStream);
        int byte2 = huffman_stream_read_byte(pStream);
        //printf("%d, %d\n", byte, byte2);
        assert_true(solution2[i][1] == byte);
        assert_true(solution2[i][0] == byte2);
    }

    huffman_stream* pStream = create_empty_test_stream();
    int extra[] = {1, 1, 1, 1, 1, 1, 1, 1, 1};
    for (int k = 0; k < 9; ++k) {
        huffman_stream_write_bit(pStream, extra[k]);
    }
    huffman_stream_pad_bits(pStream, PAD_N_ZERO);
    huffman_stream_flush(pStream);
    huffman_stream_rewind(pStream);

    int byte = huffman_stream_read_byte(pStream);
    int byte2 = huffman_stream_read_byte(pStream);
    int byte3 = huffman_stream_read_byte(pStream);
    int byte4 = huffman_stream_read_byte(pStream);
    assert_true(byte == 255);
    assert_true(byte2 == 128);
    assert_true(byte3 == 1);
    assert_true(byte4 == -1);

    return NULL;
}
