//
// Created by tobiah on 04/10/17.
//

#include "../data_structures/static_queue.h"
#include "../huffman_trees/adaptive_huffman_tree.h"
#include "adaptive_sliding_huffman.h"
void encode_adaptive_sliding_huffman(huffman_stream* input, huffman_stream* output) {
    int c_up;
    int c_down;
    int sliding_buffer[SLIDING_WINDOW_SIZE];
    path_cache cache_block;
    static_queue_int sliding_window;
    adaptive_huffman_tree tree;

    cache_block_init(&cache_block, 1);
    adaptive_huffman_tree_init(&tree);
    static_queue_init_int(&sliding_window, sliding_buffer, SLIDING_WINDOW_SIZE);

    uint64_t count = 0;
    while ((c_up = huffman_stream_read_byte(input)) != EOF) {

        adaptive_huffman_tree_encode(output, &tree, (uint8_t) c_up, &cache_block);
        adaptive_huffman_tree_update(&tree, (uint8_t) c_up);
        static_queue_push_int(&sliding_window, c_up);

        if (static_queue_size_int(&sliding_window) == SLIDING_WINDOW_SIZE) {
            c_down = static_queue_pop_int(&sliding_window);
            adaptive_huffman_tree_downdate(&tree, (uint8_t) c_down);
        }
        count++;
    }

    huffman_stream_pad_bits(output, PAD_N_ZERO);

    adaptive_huffman_tree_free(&tree);
}

void decode_adaptive_sliding_huffman(huffman_stream* input, huffman_stream* output) {
    int c_up;
    int c_down;
    int sliding_buffer[SLIDING_WINDOW_SIZE];
    static_queue_int sliding_window;
    adaptive_huffman_tree tree;

    adaptive_huffman_tree_init(&tree);
    static_queue_init_int(&sliding_window, sliding_buffer, SLIDING_WINDOW_SIZE);

    while ((c_up = adaptive_huffman_tree_decode(input, &tree)) != EOF) {

        adaptive_huffman_tree_update(&tree, (uint8_t) c_up);
        huffman_stream_write_byte(output, c_up);
        static_queue_push_int(&sliding_window, c_up);

        if (static_queue_size_int(&sliding_window) == SLIDING_WINDOW_SIZE) {
            c_down = static_queue_pop_int(&sliding_window);
            adaptive_huffman_tree_downdate(&tree, (uint8_t) c_down);
        }
    }

    adaptive_huffman_tree_free(&tree);
}

