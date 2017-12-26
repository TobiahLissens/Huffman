//
// Created by tobiah on 30/11/17.
//

#include "test_tree_consistency.h"


bool test_all_consistency(standard_huffman_tree* tree) {
    return test_check_weight_consistency(tree)
           && test_order_nr_consistency(tree)
           && test_brother_consistency(tree);
}


bool test_check_weight_consistency(standard_huffman_tree* tree) {
    bool recursive_check(huffman_node* root) {
        if (root == NULL) { return true; }
        if (root->left == NULL && root->right == NULL) { return true; }
        return root->weight == root->left->weight + root->right->weight
               && recursive_check(root->left)
               && recursive_check(root->right);
    }
    return recursive_check(tree->root);
}

bool test_order_nr_consistency(standard_huffman_tree* tree) {
    bool recursive_check(huffman_node* root, int order, uint64_t weight) {
        if (root == NULL) { return true; }

        return root->order_number > order && weight >= root->weight &&
               recursive_check(root->left, root->order_number, root->weight)
               && recursive_check(root->right, root->order_number, root->weight);;

    }
    if (tree->root == NULL) { return true; }
    return recursive_check(tree->root->left, tree->root->order_number, tree->root->weight)
           && recursive_check(tree->root->right, tree->root->order_number, tree->root->weight);
}

bool test_brother_consistency(standard_huffman_tree* tree) {
    bool recursive_check(huffman_node* root) {
        if (root == NULL) { return true; }
        if (root->left == NULL && root->right == NULL) { return true; }
        return (root->right->order_number == root->left->order_number + 1
                || root->right->order_number + 1 == root->left->order_number)
               && recursive_check(root->left)
               && recursive_check(root->right);
    }
    return recursive_check(tree->root);
}

