//
// Created by tobiah on 30/11/17.
//

#include "../../src/huffman_trees/standard_huffman_tree.h"

#ifndef HUFFMAN_PROJECT_TEST_TREE_CONSISTENCY_H
#define HUFFMAN_PROJECT_TEST_TREE_CONSISTENCY_H

bool test_all_consistency(standard_huffman_tree *tree);
bool test_check_weight_consistency(standard_huffman_tree *tree);
bool test_order_nr_consistency(standard_huffman_tree *tree);
bool test_brother_consistency(standard_huffman_tree *tree);


#endif //HUFFMAN_PROJECT_TEST_TREE_CONSISTENCY_H
