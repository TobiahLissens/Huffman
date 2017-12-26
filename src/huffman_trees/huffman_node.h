//
// Created by tobiah on 12.10.17.
//

#ifndef HUFFMAN_PROJECT_HUFFMAN_NODE_H
#define HUFFMAN_PROJECT_HUFFMAN_NODE_H

#include "../data_structures/huffman_path_cache.h"
#include "../util/io_operations.h"
#include <stdio.h>

struct static_queue_tree_node;

typedef struct huffman_node {
    //value of the node only applicable to leaves
    uint8_t value;

    // the weight of the node
    uint64_t weight;

    //the order number of th node only applicable to adaptive algorithms
    uint16_t order_number;

    //parent of the node
    struct huffman_node* parent;

    //left child of the node
    struct huffman_node* left;

    //right child of the node
    struct huffman_node* right;
} huffman_node;

/**
 * create a three_node.
 * @param value value of the tree_node.
 * @param weight the weight of the tree_node
 * @return
 */
huffman_node* huffman_node_create(uint8_t value, uint64_t weight);

/**
 *
 * @param first first tree_node.
 * @param second second tree_node
 * @return new parent node containing first and second as children.
 */
huffman_node* huffman_nodes_merge(huffman_node* first, huffman_node* second);

/**
 * checks if a node is a leaf
 * @param node node to check
 * @return 1 if the node is a leaf else 0.
 */
bool huffman_node_is_leaf(huffman_node* node);

/**
 * 1 if the node is root
 * 0 if the node is not root.
 * @param node node to be checked
 * @return see description above.
 */
bool huffman_node_is_root(huffman_node* node);

/**
 * checks if tree_node child is the child of tree_node parent.
 * @param parent tree_node parent
 * @param child tree_node child of the parent
 * @return
 */
bool huffman_node_is_child(huffman_node* parent, huffman_node* child);

/**
 * find the brother of a node
 * @param node pointer a node.
 * @return brother of the node.
 */
huffman_node* huffman_node_brother(huffman_node* node);

/**
 * find the nodes path to the root.
 * @param node the node
 * @param buffer buffer to contain the path
 * @return size of the path.
 */
void huffman_node_find_root_path(huffman_node* node, path_cache* cache);

/**
 * recursively fill in the weights of the nodes.
 * @param node pointer to the root node.
 * @return weight of the node
 */
uint64_t huffman_node_fill_weights(huffman_node* node);

/**
 * Write the tree out to the FILE breath first labelling leafs 1 and parents 0.
 * @param output FILE to write to.
 * @param node node to recursively write out to file
 * @param queue queue to push the leaf's on.
 */
void huffman_node_fwrite(huffman_stream* output, huffman_node* node, struct static_queue_tree_node* queue);

/**
 * Read a tree from the FILE.
 * @param input FILE to read from.
 * @param queue queue to push the leaf's on.
 * @return recreated tree.
 */
huffman_node* huffman_node_fread(huffman_stream* input, struct static_queue_tree_node* queue);

/**
 * print the tree's representation out to stderr.
 * @param node the node to print out recursively
 * @param tabs amount of tabs before printing the representation.
 */
void huffman_node_print(huffman_node* node, size_t tabs);

/**
 * Compare two node's by its weight.
 * @param first tree_node
 * @param second tree_node
 * @return 1 if first > second -1 if second > first else 0
 */
int huffman_node_cmp(huffman_node* first, huffman_node* second);

/**
 *
 * Compare two node's by its weight to be used with qsort.
 * @param first tree_node
 * @param second tree_node
 * @return 1 if first > second -1 if second > first else 0
 */
int huffman_node_cmp_qsort(const void* first, const void* second);

/**
 * free the node recursively
 * @param node no be freed.
 */
void huffman_node_free(huffman_node* node);

#endif //HUFFMAN_PROJECT_HUFFMAN_NODE_H

