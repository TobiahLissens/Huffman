//
// Created by tobiah on 03/10/17.
//

#ifndef HUFFMAN_PROJECT_HUFFMAN_TREE_H
#define HUFFMAN_PROJECT_HUFFMAN_TREE_H


#include "../util/bit_twiddling.h"
#include "../util/io_operations.h"
#include "huffman_node.h"
#include <stdio.h>

typedef struct standard_huffman_tree {
    //the root node
    huffman_node* root;

    //map from bytes to the leaves
    huffman_node* leaves[BYTE_RANGE];

    //amount of leaves
    size_t leaf_count;

} standard_huffman_tree;


/**
 * creates a huffman tree from a count table
 * @param word_count the amount a certain char appears in the text
 * @param word_count_size the size of the alphabet
 * @return a huffman_tree
 */
void standard_huffman_tree_init(standard_huffman_tree* tree, uint64_t* word_count, size_t word_count_size);

/**
 * checks if the tree contains char c.
 * @param tree pointer to the tree
 * @param c char to check
 * @return 1 if the tree contains c else 0
 */
bool standard_huffman_tree_contains(standard_huffman_tree* tree, uint8_t c);

/**
 * check if the tree is empty
 * @param tree pointer to the tree
 * @return true if empty else false
 */
bool standard_huffman_tree_is_empty(standard_huffman_tree* tree);

/**
 * write the tree to the output_stream
 * @param output pointer to the stream
 * @param tree pointer to the tree to write to the stream
 */
void standard_huffman_tree_fwrite(huffman_stream* output, standard_huffman_tree* tree);

/**
 * read huffman_tree from input.
 * @param input pointer to huffman_stream
 * @return huffman_tree constructed from the FILE stream
 */
void standard_huffman_tree_fread(standard_huffman_tree* tree, huffman_stream* input);

/**
 * @param input FILE to write to
 * @param tree tree used to decode.
 * @return decoded char
 */
int standard_huffman_tree_decode_byte(huffman_stream* input, standard_huffman_tree* tree);

/**
 * convert the huffman tree to cache_path's for every leaf in the tree.
 * @param tree pointer to the huffman_tree
 * @param block pointer to the path_cache_block
 */
void standard_huffman_tree_to_path_cache_block(standard_huffman_tree* tree, path_cache_block block);

/**
 * free the tree's memory resources
 * @param tree tree to be freed
 */
void standard_huffman_tree_free(standard_huffman_tree* tree);

#endif //HUFFMAN_PROJECT_HUFFMAN_TREE_H
