//
// Created by tobiah on 22.10.17.
//
#ifndef HUFFMAN_PROJECT_ADAPTIVE_HUFFMAN_NODE_H
#define HUFFMAN_PROJECT_ADAPTIVE_HUFFMAN_NODE_H

#include "adaptive_huffman_tree.h"
#include "huffman_node.h"

/**
 * swap nodes the two nodes in the tree
 * @param first tree_node* to swap
 * @param second tree_node* to swap
 */
void adaptive_huffman_swap_nodes(adaptive_huffman_tree* tree, huffman_node* first, huffman_node* second);

/**
 * check if a certain tree_node is the not coded node.
 * @param node the node to check
 * @return 1 if node is nc else 0
 */
bool adaptive_huffman_node_is_nc(huffman_node* node);

#endif //HUFFMAN_PROJECT_ADAPTIVE_HUFFMAN_NODE_H
