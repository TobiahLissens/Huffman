//
// Created by tobiah on 22.10.17.
//

#include "adaptive_huffman_node.h"


void adaptive_huffman_swap_nodes(adaptive_huffman_tree* tree, huffman_node* first, huffman_node* second) {
    NULL_CHECK(first);
    NULL_CHECK(second);

    //if the nodes are the same just return (small optimization).
    if (first == second) {
        return;
    }

    huffman_node* parent_first = first->parent;
    huffman_node* parent_second = second->parent;

    //find the side of the first child
    huffman_node** first_child_side = (parent_first->left == first) ?
                                   &parent_first->left :
                                   &parent_first->right;

    // find the side of the second child
    huffman_node** second_child_side = (parent_second->left == second) ?
                                    &parent_second->left :
                                    &parent_second->right;

    //change the pointers of the parents.
    *first_child_side = second;
    *second_child_side = first;

    //swap the parent pointers of the children
    first->parent = parent_second;
    second->parent = parent_first;

    //swap order numbers
    uint16_t first_order = first->order_number;
    first->order_number = second->order_number;
    second->order_number = first_order;

    tree->order_nr_map[first->order_number] = first;
    tree->order_nr_map[second->order_number] = second;
}

bool adaptive_huffman_node_is_nc(huffman_node* node) {
    NULL_CHECK(node);

    return node->weight == 0; //node's with weight 0 are nc
}
