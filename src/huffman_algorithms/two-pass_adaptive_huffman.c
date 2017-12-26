//
// Created by tobiah on 03/10/17.
//
#include "../huffman_trees/adaptive_huffman_tree.h"
#include "../huffman_trees/two_pass_huffman_tree.h"
#include "../util/file_cache.h"
#include "two-pass_adaptive_huffman.h"

void encode_two_pass_adaptive_huffman(huffman_stream* input, huffman_stream* output) {

    int c;
    file_cache cache;
    path_cache path_cache;
    adaptive_huffman_tree tree;
    uint64_t frequency_map[BYTE_RANGE] = {0};

    file_cache_init(&cache, input);
    count_frequencies(cache, frequency_map);
    cache_block_init(&path_cache, 1);
    two_pass_huffman_tree_init(&tree, frequency_map, BYTE_RANGE);
    two_pass_huffman_tree_fwrite(output, &tree);

    for (int j = 0; j < file_cache_size(&cache); ++j) {
        c = file_cache_get(&cache, (size_t) j);
        adaptive_huffman_tree_encode(output, &tree, (uint8_t) c, &path_cache);
        adaptive_huffman_tree_downdate(&tree, (uint8_t) c);
    }

    huffman_stream_pad_bits(output, PAD_N_ZERO);
    file_cache_free(&cache);
    adaptive_huffman_tree_free(&tree);
}

void decode_two_pass_adaptive_huffman(huffman_stream* input, huffman_stream* output) {
    int c;
    adaptive_huffman_tree tree;

    two_pass_huffman_tree_fread(&tree, input);

    while ((c = adaptive_huffman_tree_decode(input, &tree)) != EOF) {
        huffman_stream_write_byte(output, c);
        adaptive_huffman_tree_downdate(&tree, (uint8_t) c);
    }

    adaptive_huffman_tree_free(&tree);
}

