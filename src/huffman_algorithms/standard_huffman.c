
#include "../huffman_trees/standard_huffman_tree.h"
#include "../util/file_cache.h"
#include "standard_huffman.h"
#include "../huffman_trees/standard_huffman_decode_tree.h"

//
// Created by tobiah on 03/10/17.
//

void encode_standard_huffman(huffman_stream* input, huffman_stream* output) {
    standard_huffman_tree tree;
    file_cache cache;
    path_cache path_cache[BYTE_RANGE];
    uint64_t frequency_map[BYTE_RANGE] = {0};

    file_cache_init(&cache, input);
    count_frequencies(cache, frequency_map);
    standard_huffman_tree_init(&tree, frequency_map, BYTE_RANGE);
    cache_block_init(path_cache, BYTE_RANGE);
    standard_huffman_tree_to_path_cache_block(&tree, path_cache);
    standard_huffman_tree_fwrite(output, &tree);

    for (size_t i = 0; i < file_cache_size(&cache); ++i) {
        path_cache_fwrite(output, path_cache + file_cache_get(&cache, i));
    }

    huffman_stream_pad_bits(output, PAD_N_ZERO);

    file_cache_free(&cache);
    standard_huffman_tree_free(&tree);
}

void decode_standard_huffman(huffman_stream* input, huffman_stream* output) {
    int c;
    path_cache cache;
    standard_huffman_tree tree;
    standard_huffman_decode_tree decode_tree;


    standard_huffman_tree_fread(&tree, input);

    path_cache_init(&cache, BYTE_SIZE);
    standard_huffman_decode_tree_init(&tree, &decode_tree);

    while ((c = standard_huffman_decode_tree_decode(&decode_tree, input, &cache)) != EOF) {
        huffman_stream_write_byte(output, c);
    }

    standard_huffman_tree_free(&tree);
    standard_huffman_decode_tree_free(&decode_tree);
}
