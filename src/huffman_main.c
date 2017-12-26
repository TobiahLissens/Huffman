#include "huffman_algorithms/adaptive_huffman.h"
#include "huffman_algorithms/adaptive_sliding_huffman.h"
#include "huffman_algorithms/blockwise_adaptive_huffman.h"
#include "huffman_algorithms/standard_huffman.h"
#include "huffman_algorithms/two-pass_adaptive_huffman.h"
#include "util/help_page.h"
#include "util/settings.h"
#include <getopt.h>

/**
 * Generic huffman encoder/decoder function
 */

typedef void (* huffman_function)(huffman_stream*, huffman_stream*);


/**
 * Encode functions
 */
const static huffman_function encode[] = {
        encode_standard_huffman,
        encode_adaptive_huffman,
        encode_adaptive_sliding_huffman,
        encode_two_pass_adaptive_huffman,
        encode_blockwise_adaptive_huffman
};

/**
 * Decode functions
 */
const static huffman_function decode[] = {
        decode_standard_huffman,
        decode_adaptive_huffman,
        decode_adaptive_sliding_huffman,
        decode_two_pass_adaptive_huffman,
        decode_blockwise_adaptive_huffman
};

const static huffman_function* exec[] = {
        encode,
        decode
};
size_t size = 0;
int main(int argc, char** argv) {
    int type = 0;
    int encode = 0;
    int c;

    FILE* input_stream = stdin;
    FILE* output_stream = stdout;

    while ((c = getopt(argc, argv, "ht:dci:o:s:")) != -1) {
        switch (c) {
            case 'h':
                printf("%s", HELP);
                exit(0);
            case 't':
                type = (int) (strtol(optarg, NULL, 10) - 1);
                if (type < 0 || type > 4) { exit(1); }
                break;
            case 'd':
                encode = 1;
                break;
            case 'c':
                encode = 0;
                break;
            case 'i':
                input_stream = fopen(optarg, "r");
                break;
            case 'o':
                output_stream = fopen(optarg, "w");
                break;
            case 's':
                size = (size_t) strtol(optarg, NULL, 10);
                break;
            default:
                THROW_ERROR(ILLEGAL_ARGUMENT, "argument is unknown");
        }
    }
    if (input_stream == NULL || output_stream == NULL) {
        THROW_ERROR(ILLEGAL_ARGUMENT, "file not found");
    }

    huffman_stream in;
    huffman_stream out;

    huffman_stream_init(input_stream, &in);
    huffman_stream_init(output_stream, &out);

    exec[encode][type](&in, &out);

    //flush out stream by closing it
    huffman_stream_close(&out);

    fclose(input_stream);
    fclose(output_stream);

    return 0;
}