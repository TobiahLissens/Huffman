//
// Created by tobiah on 28.10.17.
//

#ifndef HUFFMAN_PROJECT_TWO_PASS_HUFFMAN_TREE_H
#define HUFFMAN_PROJECT_TWO_PASS_HUFFMAN_TREE_H

#include "adaptive_huffman_tree.h"

/**
 * fill in the order_numbers of a huffman_tree
 * @param tree pointer to the adaptive_huffman_tree
 */
void two_pass_fill_order_numbers(adaptive_huffman_tree* tree);

/**
 * initialise the adaptive huffman tree for the two_pass huffman algorithm
 * @param tree pointer to the tree that needs to be initialised.
 * @param frequency_map the counted frequencies of every byte
 * @param length the amount of possible bytes
 */
void two_pass_huffman_tree_init(adaptive_huffman_tree* tree, uint64_t* frequency_map, size_t length);

/**
 * write the adaptive_huffman_tree to the stream together with its frequencies
 * @param output pointer to the stream to write to
 * @param tree pointer to the tree to write to the stream
 */
void two_pass_huffman_tree_fwrite(huffman_stream* output, adaptive_huffman_tree* tree);

/**
 * read an adaptive_huffman_tree from the stream
 * @param tree pointer to the proto tree
 * @param input pointer to the stream to be read from
 */
void two_pass_huffman_tree_fread(adaptive_huffman_tree* tree, huffman_stream* input);


#endif //HUFFMAN_PROJECT_TWO_PASS_HUFFMAN_TREE_H
