//
// Created by tobiah on 14.10.17.
//

#include "adaptive_huffman_node.h"

void adaptive_huffman_tree_init(adaptive_huffman_tree* tree) {
    NULL_CHECK(tree);

    memset(tree->tree.leaves, 0, sizeof(*tree->tree.leaves) * BYTE_RANGE);
    memset(tree->order_nr_map, 0, sizeof(*tree->order_nr_map) * BYTE_RANGE);

    tree->tree.root = huffman_node_create('0', 0);
    tree->not_coded = tree->tree.root;
    tree->not_coded->order_number = 0;
    tree->tree.leaf_count = 1; //set leaf_count to 1 because of not_coded
}

void adaptive_huffman_tree_update(adaptive_huffman_tree* tree, uint8_t c) {
    NULL_CHECK(tree);

    huffman_node* t_next;
    huffman_node* t = tree->tree.leaves[c];

    if (!standard_huffman_tree_contains(&tree->tree, c)) {
        t = adaptive_tree_add_byte(tree, c);
        if (t == NULL) { return; }
    }

    while (!huffman_node_is_root(t)) {

        t_next = adaptive_huffman_tree_find_next(tree, t);

        if (!huffman_node_is_child(t_next, t)) {
            adaptive_huffman_swap_nodes(tree, t, t_next);
        }

        t->weight++;
        t = t->parent;
    }
    t->weight++;
}


void adaptive_huffman_tree_downdate(adaptive_huffman_tree* tree, uint8_t c) {
    NULL_CHECK(tree);

    huffman_node* t = tree->tree.leaves[c];
    huffman_node* t_prev;

    while (!huffman_node_is_root(t)) {
        t_prev = adaptive_huffman_tree_find_prev(tree, t);

        adaptive_huffman_swap_nodes(tree, t, t_prev);

        t->weight--;
        t = t->parent;
    }
    t->weight--;

    if (tree->tree.leaves[c]->weight == 0) {
        adaptive_tree_remove_byte(tree, c);
    }
}

void adaptive_huffman_tree_encode(huffman_stream* output, adaptive_huffman_tree* tree, uint8_t c, path_cache* path) {
    NULL_CHECK(output);
    NULL_CHECK(tree);
    NULL_CHECK(path);

    huffman_node* start = standard_huffman_tree_contains(&tree->tree, c) ? tree->tree.leaves[c] : tree->not_coded;

    huffman_node_find_root_path(start, path);
    path_cache_fwrite(output, path);

    if (adaptive_huffman_node_is_nc(start)) {
        huffman_stream_write_n_bits(output, c, BYTE_SIZE);
    }
}

int adaptive_huffman_tree_decode(huffman_stream* input, adaptive_huffman_tree* tree) {
    NULL_CHECK(input);
    NULL_CHECK(tree);

    huffman_node* node = tree->tree.root;
    int bit;

    if (standard_huffman_tree_is_empty(&tree->tree)) { return EOF; }

    if (node == NULL) { return EOF; }

    while (!huffman_node_is_leaf(node)) {
        bit = huffman_stream_read_bit(input);
        if (bit == EOF) { return EOF; }
        node = bit ? node->right : node->left;
    }

    if (adaptive_huffman_node_is_nc(node)) {
        uint64_t byte = 0;
        if (huffman_stream_read_n_bits(input, &byte, BYTE_SIZE) != BYTE_SIZE) return EOF;
        return (uint8_t) byte;
    }

    return node->value;
}

huffman_node* adaptive_huffman_tree_find_next(adaptive_huffman_tree* tree, huffman_node* t) {
    NULL_CHECK(tree);
    NULL_CHECK(t);
    huffman_node* next = t;

    while (next->order_number != 0
           && t->weight == tree->order_nr_map[next->order_number - 1]->weight) {
        next = tree->order_nr_map[next->order_number - 1];
    }

    return next;
}

huffman_node* adaptive_huffman_tree_find_prev(adaptive_huffman_tree* tree, huffman_node* t) {
    NULL_CHECK(tree);
    NULL_CHECK(t);

    int max = (int) ((tree->tree.leaf_count - 1) * 2); //calculate the highest order number
    huffman_node* next = t;

    while (next->order_number < max
           && t->weight == tree->order_nr_map[next->order_number + 1]->weight) {
        next = tree->order_nr_map[next->order_number + 1];
    }

    return next;
}

huffman_node* adaptive_tree_add_byte(adaptive_huffman_tree* tree, uint8_t byte) {
    NULL_CHECK(tree);
    tree->tree.leaf_count++;

    huffman_node* not_coded = tree->not_coded;
    huffman_node* old_parent = not_coded->parent;
    huffman_node* new_node = huffman_node_create(byte, 1);
    huffman_node* new_parent = huffman_nodes_merge(not_coded, new_node);
    uint16_t last_order_number = tree->not_coded->order_number;

    new_parent->parent = old_parent;
    tree->tree.leaves[byte] = new_node;

    //update the order numbers.
    new_parent->order_number = last_order_number++;
    new_node->order_number = last_order_number++;
    not_coded->order_number = last_order_number;

    //add tree_nodes to order_nr_map
    tree->order_nr_map[new_parent->order_number] = new_parent;
    tree->order_nr_map[new_node->order_number] = new_node;
    tree->order_nr_map[not_coded->order_number] = not_coded;

    //if the new parent is root set it as the tree's root
    if (huffman_node_is_root(new_parent)) {
        tree->tree.root = new_parent;
        return NULL;
    }

    huffman_node* t = new_parent->parent;
    huffman_node** nc_place = adaptive_huffman_node_is_nc(t->left) ? &t->left : &t->right;
    *nc_place = new_parent;

    return new_parent->parent;
}

void adaptive_tree_remove_byte(adaptive_huffman_tree* tree, uint8_t byte) {
    NULL_CHECK(tree);

    huffman_node* node = tree->tree.leaves[byte];
    //parent to remove
    huffman_node* parent = node->parent;
    //brother to take over the place of the parent
    huffman_node* brother = huffman_node_brother(node);

    tree->tree.leaf_count--;

    //check if parent is null
    if (huffman_node_is_root(parent)) {
        //make sure the tree is populated with at least 3 nodes
        return;
    }

    huffman_node* parent_parent = parent->parent;

    huffman_node** child = parent_parent->left == parent
                           ? &parent_parent->left : &parent_parent->right;
    *child = brother;

    brother->parent = parent_parent;
    brother->order_number = parent->order_number;
    tree->order_nr_map[brother->order_number] = brother;
    tree->tree.leaves[node->value] = NULL;

    if (tree->not_coded != NULL && brother->order_number >= 2) {
        huffman_node* swap = tree->order_nr_map[brother->order_number - 2];
        adaptive_huffman_swap_nodes(tree, parent_parent, swap);
    }

    //node to remove
    free(parent);
    free(node);
}

void adaptive_huffman_tree_reset(adaptive_huffman_tree* tree) {
    NULL_CHECK(tree);

    adaptive_huffman_tree_free(tree);
    adaptive_huffman_tree_init(tree);
}

void adaptive_huffman_tree_free(adaptive_huffman_tree* tree) {
    NULL_CHECK(tree);

    huffman_node_free(tree->tree.root);
}



