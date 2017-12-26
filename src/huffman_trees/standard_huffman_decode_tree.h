//
// Created by tobiah on 28/11/17.
//

#ifndef HUFFMAN_PROJECT_STANDAARD_HUFFMAN_DECODE_TREE_H
#define HUFFMAN_PROJECT_STANDAARD_HUFFMAN_DECODE_TREE_H


#include <stdint.h>
#include "../util/bit_twiddling.h"
#include "standard_huffman_tree.h"

typedef struct standard_huffman_decode_leaf {

    //decoded char
    uint8_t c;

    //length of code needed to decode
    size_t length;

} standard_huffman_decode_leaf;

typedef struct standard_huffman_decode_node {

    //0 if leaf 1 if internal node.
    uint8_t type;

    //pointer to the value of the tree.
    void* value;

} standard_huffman_decode_node;

typedef struct standard_huffman_decode_tree {

    //list of nodes
    struct standard_huffman_decode_node nodes[BYTE_RANGE];

} standard_huffman_decode_tree;

/**
 * initialize the standard_huffman_decode_tree with the original standard_uffman_tree
 * @param tree pointer to the original standard_huffman_tree
 * @param decode_tree
 */
void standard_huffman_decode_tree_init(standard_huffman_tree* tree, standard_huffman_decode_tree* decode_tree);

/**
 *
 * @param tree pointer to the decoding tree
 * @param stream pointer to the huffman_stream
 * @param cache pointer to the path_cache caching the non_used bits
 * @return decoded byte or EOF
 */
int standard_huffman_decode_tree_decode(standard_huffman_decode_tree* tree,
                                        huffman_stream* stream,
                                        path_cache* cache);

/**
 * free all the resources used by the standard_huffman_decode_tree
 * @param tree pointer to the standard_huffman_decoding_tree
 */
void standard_huffman_decode_tree_free(standard_huffman_decode_tree* tree);

#endif //HUFFMAN_PROJECT_STANDAARD_HUFFMAN_DECODE_TREE_H
