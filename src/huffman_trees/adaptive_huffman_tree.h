//
// Created by tobiah on 14.10.17.
//

#ifndef HUFFMAN_PROJECT_ADAPTIVE_HUFFMAN_TREE_H
#define HUFFMAN_PROJECT_ADAPTIVE_HUFFMAN_TREE_H

#include "standard_huffman_tree.h"

#define MAX_NODE_COUNT (BYTE_RANGE * 2 + 1) //equals 513

typedef struct adaptive_huffman_tree {

    //reuse the normal standard_huffman_tree structures.
    standard_huffman_tree tree;

    //map to order number.
    huffman_node* order_nr_map[MAX_NODE_COUNT];

    //represents the not coded char's in the tree.
    huffman_node* not_coded;

} adaptive_huffman_tree;

/**
 * initialize the adaptive huffman_tree
 * @param tree pointer to an uninitialized adaptive_huffman_tree
 */
void adaptive_huffman_tree_init(adaptive_huffman_tree* tree);

/**
 * increase the weight of the byte
 * and rebalance the tree
 * @param tree tree to rebalance
 * @param c char's weight to update
 */
void adaptive_huffman_tree_update(adaptive_huffman_tree* tree, uint8_t c);

/**
 * decrease the weight of the byte
 * and rebalance the tree
 * @param tree tree to rebalance
 * @param c char's weight to update
 */
void adaptive_huffman_tree_downdate(adaptive_huffman_tree* tree, uint8_t c);

/**
 * encode a byte with the adaptive huffman tree
 * @param output output file
 * @param tree tree used for encoding
 * @param c character to encode.
 * @param path BUFFER used for encoding.
 */
void adaptive_huffman_tree_encode(huffman_stream* output, adaptive_huffman_tree* tree, uint8_t c, path_cache* path);

/**
 * decode a byte with the adaptive huffman tree.
 * @param input input file.
 * @param tree tree used for decoding.
 * @return decoded byte.
 */
int adaptive_huffman_tree_decode(huffman_stream* input, adaptive_huffman_tree* tree);

/**
 * finds the node t should swap with.
 * in other words find the node with the same weight and the lowest order number in the tree.
 * @param tree tree containing node t
 * @param t pointer to the node t
 * @return the node t should swap with in the next tree fix.
 */
huffman_node* adaptive_huffman_tree_find_next(adaptive_huffman_tree* tree, huffman_node* t);

/**
 * finds the node t should swap with
 * in other words find the node with the same weight and highest order number in the tree.
 * @param tree tree containing node t
 * @param t pointer to the node t
 * @return the node t should swap with in the next tree fix.
 */
huffman_node* adaptive_huffman_tree_find_prev(adaptive_huffman_tree* tree, huffman_node* t);

/**
 * add an unknown byte to the tree
 * @param tree tree to add the byte to
 * @param byte byte to add to the tree
 * @return parent of the nc_tree_node
 */
huffman_node* adaptive_tree_add_byte(adaptive_huffman_tree* tree, uint8_t byte);

/**
 * remove a byte from the tree
 * @param tree the tree the byte is removed from
 * @param byte byte to remove from the tree
 * @return parent of the nc_tree_node;
 */
void adaptive_tree_remove_byte(adaptive_huffman_tree* tree, uint8_t byte);

/**
 * free's the allocated resources and reinitializes the tree.
 * @param tree tree to be reset
 */
void adaptive_huffman_tree_reset(adaptive_huffman_tree* tree);

/**
 * free the tree.
 * @param tree
 */
void adaptive_huffman_tree_free(adaptive_huffman_tree* tree);

#endif //HUFFMAN_PROJECT_ADAPTIVE_HUFFMAN_TREE_H
