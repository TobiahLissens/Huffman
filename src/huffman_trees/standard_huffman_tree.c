//
// Created by tobiah on 03/10/17.
//

#include "../data_structures/static_queue.h"
#include "standard_huffman_tree.h"

void standard_huffman_tree_init(standard_huffman_tree* tree, uint64_t* word_count, size_t word_count_size) {
    NULL_CHECK(tree);
    NULL_CHECK(word_count);

    tree->leaf_count = 0;

    //count the used nodes
    for (size_t i = 0; i < word_count_size; ++i) {
        if (word_count[i]) {
            tree->leaf_count++;
        }
    }

    //in case of only 1 letter used just add the first letter with count 0
    //this is either length 0 or 1
    if (tree->leaf_count == 1) {
        word_count[0] > word_count[1] ? word_count[1]++ : word_count[0]++;
        tree->leaf_count++;
    }

    static_queue_tree_node proto_initial_queue;
    static_queue_tree_node proto_second_queue;
    huffman_node* initial_queue_array[tree->leaf_count];
    huffman_node* second_queue_array[tree->leaf_count];

    static_queue_init_tree_node(&proto_initial_queue, initial_queue_array, tree->leaf_count);
    static_queue_init_tree_node(&proto_second_queue, second_queue_array, tree->leaf_count);

    for (size_t i = 0; i < word_count_size; ++i) {
        if (word_count[i] != 0) {
            huffman_node* leaf = huffman_node_create((uint8_t) i, word_count[i]);
            static_queue_push_tree_node(&proto_initial_queue, leaf);
            tree->leaves[i] = leaf;
        } else {
            tree->leaves[i] = NULL;
        }
    }

    //sort the tree_nodes
    qsort(proto_initial_queue.values, tree->leaf_count, sizeof(huffman_node*), huffman_node_cmp_qsort);

    //build the standard_huffman_tree
    huffman_node* first;
    huffman_node* second;
    huffman_node* parent;

    while (static_queue_size_tree_node(&proto_initial_queue) + static_queue_size_tree_node(&proto_second_queue) > 1) {
        first = static_queue_pop_lowest_tree_node(huffman_node_cmp, &proto_initial_queue, &proto_second_queue);
        second = static_queue_pop_lowest_tree_node(huffman_node_cmp, &proto_initial_queue, &proto_second_queue);
        parent = huffman_nodes_merge(first, second);
        static_queue_push_tree_node(&proto_second_queue, parent);
    }

    tree->root = static_queue_size_tree_node(&proto_second_queue)
                 ? static_queue_pop_tree_node(&proto_second_queue) : NULL;

}

bool standard_huffman_tree_contains(standard_huffman_tree* tree, uint8_t c) {
    NULL_CHECK(tree);

    return tree->leaves[c] != NULL;
}

bool standard_huffman_tree_is_empty(standard_huffman_tree* tree) {
    NULL_CHECK(tree);

    return tree->root == NULL;
}

void standard_huffman_tree_fwrite(huffman_stream* output, standard_huffman_tree* tree) {
    NULL_CHECK(output);
    NULL_CHECK(tree);

    huffman_node* proto_queue[tree->leaf_count];
    static_queue_tree_node queue;
    static_queue_init_tree_node(&queue, proto_queue, tree->leaf_count);

    huffman_node_fwrite(output, tree->root, &queue);

    //pad to a multiple of 8 bits
    huffman_stream_pad_bits(output, PAD);

    while (static_queue_size_tree_node(&queue)) {
        huffman_stream_write_byte(output, static_queue_pop_tree_node(&queue)->value);
    }
}

void standard_huffman_tree_fread(standard_huffman_tree* tree, huffman_stream* input) {
    NULL_CHECK(tree);
    NULL_CHECK(input);

    huffman_node* proto_queue[BYTE_RANGE];
    static_queue_tree_node queue;

    static_queue_init_tree_node(&queue, proto_queue, BYTE_RANGE);

    tree->root = huffman_node_fread(input, &queue);

    //pad the bits to a multiple of 8
    huffman_stream_unpad_bits(input);

    tree->leaf_count = static_queue_size_tree_node(&queue);

    if (tree->leaf_count > BYTE_RANGE) {THROW_ERROR(ILLEGAL_STATE, "Invalid tree size");}

    memset(tree->leaves, 0, sizeof(*tree->leaves) * BYTE_RANGE);

    while (static_queue_size_tree_node(&queue)) {

        int c = huffman_stream_read_byte(input);
        if (c == EOF) { THROW_ERROR(ILLEGAL_STATE, "unexpected EOF"); }
        tree->leaves[c] = static_queue_pop_tree_node(&queue);
        tree->leaves[c]->value = (uint8_t) c;
    }
}

int standard_huffman_tree_decode_byte(huffman_stream* input, standard_huffman_tree* tree) {
    NULL_CHECK(input);
    NULL_CHECK(tree);

    huffman_node* node = tree->root;
    int bit;

    while ((bit = huffman_stream_read_bit(input)) != EOF) {

        node = bit ? node->right : node->left;

        if (huffman_node_is_leaf(node)) {
            return node->value;
        }
    }

    return EOF;
}

void standard_huffman_tree_to_path_cache_block(standard_huffman_tree* tree, path_cache* cache_block) {
    NULL_CHECK(tree);
    NULL_CHECK(cache_block);

    for (int c = 0; c < BYTE_RANGE; ++c) {

        if (tree->leaves[c] != NULL) {
            huffman_node_find_root_path(tree->leaves[c], cache_block + c);
        }
    }
}

void standard_huffman_tree_free(standard_huffman_tree* tree) {
    NULL_CHECK(tree);

    huffman_node_free(tree->root);
}



