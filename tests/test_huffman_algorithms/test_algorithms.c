//
// Created by tobiah on 17/10/17.
//


#include <dirent.h>
#include <memory.h>
#include "test_algorithms.h"
#include "../../src/huffman_algorithms/standard_huffman.h"
#include "../../src/huffman_algorithms/adaptive_huffman.h"
#include "../../src/huffman_algorithms/adaptive_sliding_huffman.h"
#include "../../src/huffman_algorithms/two-pass_adaptive_huffman.h"

#include "../../src/huffman_algorithms/blockwise_adaptive_huffman.h"

#define TEMP_FILE_1 "_tmp.out" //could have used tmp file function
#define TEMP_FILE_2 "_tmp2.out"

const static char* TEST_DIR = "../tests/test_files/";

char* test_all_huffman_algorithms() {

    //encoder decoder
    test(test_standard_huffman);
    test(test_adaptive_huffman);
    test(test_sliding_window_adaptive_huffman);
    test(test_two_pass_huffman);
    test(test_blockwise_adaptive_huffman);

    //decoder robustness
    test(test_decode_robustness_standaard_huffman);
    test(test_decode_robustness_adaptive_huffman);
    test(test_decode_robustness_sliding_window_huffman);
    test(test_decode_robustness_two_pass_huffman);
    test(test_decode_robustness_blockwise_huffman);

    return NULL;
}

char* test_huffman_algorithm(huffman_function encoder, huffman_function decoder) {
    char string_buf[200];
    DIR* map = opendir(TEST_DIR);

    struct dirent* dp;
    while ((dp = readdir(map)) != NULL) {
        if (dp->d_type == DT_REG) {
            memset(string_buf, 0, 200);
            strcat(string_buf, TEST_DIR);
            strcat(string_buf, dp->d_name);

            FILE* input_stream = fopen(string_buf, "r+");
            FILE* output_stream1 = fopen(TEMP_FILE_1, "w+");
            FILE* output_stream2 = fopen(TEMP_FILE_2, "w+");

            huffman_stream in;
            huffman_stream out1;
            huffman_stream out2;

            huffman_stream_init(input_stream, &in);
            huffman_stream_init(output_stream1, &out1);
            huffman_stream_init(output_stream2, &out2);

            encoder(&in, &out1);

            huffman_stream_flush(&out1);
            huffman_stream_rewind(&out1);

            decoder(&out1, &out2);

            huffman_stream_flush(&out2);
            huffman_stream_rewind(&out2);
            huffman_stream_rewind(&in);

            int cmp = cmp_file(input_stream, output_stream2);

            assert_true(cmp);

            huffman_stream_close(&in);
            huffman_stream_close(&out1);
            huffman_stream_close(&out2);

            fclose(input_stream);
            fclose(output_stream1);
            fclose(output_stream2);

            remove(TEMP_FILE_1);
            remove(TEMP_FILE_2);
        }
    }
    closedir(map);
    return NULL;
}

char* test_standard_huffman() {
    return test_huffman_algorithm(encode_standard_huffman, decode_standard_huffman);
}

char* test_adaptive_huffman() {
    return test_huffman_algorithm(encode_adaptive_huffman, decode_adaptive_huffman);
}

char* test_sliding_window_adaptive_huffman() {
    return test_huffman_algorithm(encode_adaptive_sliding_huffman, decode_adaptive_sliding_huffman);
}

char* test_two_pass_huffman() {
    return test_huffman_algorithm(encode_two_pass_adaptive_huffman, decode_two_pass_adaptive_huffman);
}

char* test_blockwise_adaptive_huffman() {
    return test_huffman_algorithm(encode_blockwise_adaptive_huffman, decode_blockwise_adaptive_huffman);
}

char* test_decode_robustness_algorithm(huffman_function decode) {
    NULL_CHECK(decode);
    char string_buf[200];
    DIR* map = opendir(TEST_DIR);

    struct dirent* dp;
    while ((dp = readdir(map)) != NULL) {
        if (dp->d_type == DT_REG) {

            memset(string_buf, 0, 200);
            strcat(string_buf, TEST_DIR);
            strcat(string_buf, dp->d_name);

            FILE* input = fopen(string_buf, "r+");
            FILE* output = fopen("/dev/null", "w");
            
            huffman_stream in;
            huffman_stream out;

            huffman_stream_init(input, &in);
            huffman_stream_init(output, &out);

            //check for errors
            if (!setjmp(buf)){
                decode(&in, &out);
            }else{
                //error codes are okay here.
                success();
            }

            //only close the output file (it only flushes the stream)
            huffman_stream_close(&out);

            fclose(input);
            fclose(output);
        }
    }
    closedir(map);

    return NULL;
}

char* test_decode_robustness_standaard_huffman() {
    return test_decode_robustness_algorithm(decode_standard_huffman);
}

char* test_decode_robustness_adaptive_huffman() {
    return test_decode_robustness_algorithm(decode_adaptive_huffman);
}

char* test_decode_robustness_sliding_window_huffman() {
    return test_decode_robustness_algorithm(decode_adaptive_sliding_huffman);
}

char* test_decode_robustness_two_pass_huffman() {
    return test_decode_robustness_algorithm(decode_two_pass_adaptive_huffman);
}

char* test_decode_robustness_blockwise_huffman() {
    return test_decode_robustness_algorithm(decode_blockwise_adaptive_huffman);
}
