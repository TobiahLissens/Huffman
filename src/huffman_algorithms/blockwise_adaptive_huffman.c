//
// Created by tobiah on 04/10/17.
//

#include "../huffman_trees/adaptive_huffman_tree.h"
#include "blockwise_adaptive_huffman.h"
void encode_blockwise_adaptive_huffman(huffman_stream* input, huffman_stream* output) {
    int c;
    int count = 0;
    path_cache cache_block;
    adaptive_huffman_tree tree;

    cache_block_init(&cache_block, 1);
    adaptive_huffman_tree_init(&tree);

    while ((c = huffman_stream_read_byte(input)) != EOF) {

        adaptive_huffman_tree_encode(output, &tree, (uint8_t) c, &cache_block);
        adaptive_huffman_tree_update(&tree, (uint8_t) c);

        if (count >= BLOCK_SIZE) {
            adaptive_huffman_tree_reset(&tree);
            count = 0;
        }

        count++;
    }

    huffman_stream_pad_bits(output, PAD_N_ZERO);

    adaptive_huffman_tree_free(&tree);
}

void decode_blockwise_adaptive_huffman(huffman_stream* input, huffman_stream* output) {
    int c;
    int count = 0;
    adaptive_huffman_tree tree;

    adaptive_huffman_tree_init(&tree);

    while ((c = adaptive_huffman_tree_decode(input, &tree)) != EOF) {

        huffman_stream_write_byte(output, c);
        adaptive_huffman_tree_update(&tree, (uint8_t) c);

        if (count >= BLOCK_SIZE) {
            adaptive_huffman_tree_reset(&tree);
            count = 0;
        }

        count++;
    }

    adaptive_huffman_tree_free(&tree);
}

