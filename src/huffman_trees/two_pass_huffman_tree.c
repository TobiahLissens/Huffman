//
// Created by tobiah on 28.10.17.
//

#include "../data_structures/static_queue.h"
#include "adaptive_huffman_node.h"


void two_pass_fill_order_numbers(adaptive_huffman_tree* tree) {
    uint16_t count = 0;
    static_queue_tree_node queue;
    huffman_node* proto_queue[tree->tree.leaf_count];

    static_queue_init_tree_node(&queue, proto_queue, tree->tree.leaf_count);

    if (tree->tree.root != NULL) {
        static_queue_push_tree_node(&queue, tree->tree.root);
    }

    while (static_queue_size_tree_node(&queue) > 0) {
        huffman_node* node = static_queue_pop_tree_node(&queue);
        tree->order_nr_map[count] = node;
        node->order_number = count++;
        if (!huffman_node_is_leaf(node)) {
            static_queue_push_tree_node(&queue, node->right);
            static_queue_push_tree_node(&queue, node->left);
        }
    }
}

void two_pass_huffman_tree_init(adaptive_huffman_tree* tree, uint64_t* frequency_map, size_t length) {
    NULL_CHECK(tree);
    NULL_CHECK(frequency_map);

    memset(tree->order_nr_map, 0, MAX_NODE_COUNT * sizeof(*tree->order_nr_map));

    // we need to explicitly set not_coded to null
    // this way the adaptive_huffman_down_date method will not do unnecessary extra swaps
    tree->not_coded = NULL;

    //use standard huffman init
    standard_huffman_tree_init(&tree->tree, frequency_map, length);

    //fill in the order numbers with a breath first algorithm
    two_pass_fill_order_numbers(tree);
}

void two_pass_huffman_tree_fwrite(huffman_stream* output, adaptive_huffman_tree* tree) {
    NULL_CHECK(output);
    NULL_CHECK(tree);

    huffman_node* proto_queue[tree->tree.leaf_count];
    static_queue_tree_node queue;

    static_queue_init_tree_node(&queue, proto_queue, tree->tree.leaf_count);

    // write out the tree structure
    huffman_node_fwrite(output, tree->tree.root, &queue);
    huffman_stream_pad_bits(output, PAD);

    // write out the value's in leaf order
    for (int i = 0; i < static_queue_size_tree_node(&queue); ++i) {
        huffman_node* node = static_queue_get_tree_node(&queue, i);
        huffman_stream_write_byte(output, node->value);
    }

    // write out the weights in leaf order
    for (int i = 0; i < static_queue_size_tree_node(&queue); ++i) {
        huffman_node* node = static_queue_get_tree_node(&queue, i);
        huffman_stream_write_n_bits(output, ((uint32_t) node->weight), 32);
    }
}


void two_pass_huffman_tree_fread(adaptive_huffman_tree* tree, huffman_stream* input) {
    NULL_CHECK(tree);
    NULL_CHECK(input);

    memset(tree->order_nr_map, 0, MAX_NODE_COUNT * sizeof(*tree->order_nr_map));
    memset(tree->tree.leaves, 0, BYTE_RANGE * sizeof(*tree->tree.leaves));

    static_queue_tree_node queue;
    huffman_node* proto_queue[BYTE_RANGE];

    static_queue_init_tree_node(&queue, proto_queue, BYTE_RANGE);


    // read in the tree structure.
    tree->not_coded = NULL;
    tree->tree.root = huffman_node_fread(input, &queue);
    tree->tree.leaf_count = static_queue_size_tree_node(&queue);

    //a tree with 1 leaf or more than 256 leafs are not compatible
    if (tree->tree.leaf_count > BYTE_RANGE || tree->tree.leaf_count == 1) {
        THROW_ERROR(ILLEGAL_STATE, "illegal node count.");
    }

    //un pad the padded bits
    huffman_stream_unpad_bits(input);

    // read in the value's in leaf order
    for (int i = 0; i < static_queue_size_tree_node(&queue); ++i) {
        int c = huffman_stream_read_byte(input);
        if (c == EOF) { THROW_ERROR(ILLEGAL_STATE, "Unexpected EOF!"); }
        huffman_node* node = static_queue_get_tree_node(&queue, i);
        node->value = (uint8_t) c;
        tree->tree.leaves[node->value] = node;
    }

    // read in the weight's in leaf order.
    for (int j = 0; j < static_queue_size_tree_node(&queue); ++j) {
        uint64_t weight = 0;
        if (huffman_stream_read_n_bits(input, &weight, 32) != 32) { THROW_ERROR(ILLEGAL_ARGUMENT, "unexpected EOF"); };
        huffman_node* node = static_queue_get_tree_node(&queue, j);
        node->weight = weight;
    }

    //fill in the rest of the weights and order numbers.
    huffman_node_fill_weights(tree->tree.root);
    two_pass_fill_order_numbers(tree);
}
