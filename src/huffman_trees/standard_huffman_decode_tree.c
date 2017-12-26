//
// Created by tobiah on 28/11/17.
//

#include "standard_huffman_decode_tree.h"
#include "../util/memory_allocations.h"

void standard_huffman_decode_tree_init(standard_huffman_tree* tree, standard_huffman_decode_tree* decode_tree) {
    path_cache block;

    memset(decode_tree->nodes, 0, BYTE_RANGE * sizeof(*decode_tree->nodes));

    for (int c = 0; c < BYTE_RANGE; ++c) {

        if (tree->leaves[c] != NULL) {

            standard_huffman_decode_tree* current_decode_tree = decode_tree;
            huffman_node_find_root_path(tree->leaves[c], &block);

            int n = (int) block.length;

            while (n > 0) {

                //check if code fits into 1 byte
                if (n <= BYTE_SIZE) {
                    //let all leaf's point to this node
                    int shift = BYTE_SIZE - n;

                    uint8_t index = (uint8_t) (block.line << shift);

                    int max = index + (1 << shift); //max permutations

                    standard_huffman_decode_leaf* leaf = SAFE_MALLOC(sizeof(standard_huffman_decode_leaf));
                    leaf->length = (size_t) n;
                    leaf->c = (uint8_t) c;

                    //all lower bit permutions
                    for (size_t i = index; i < max; ++i) {
                        standard_huffman_decode_node* node = &current_decode_tree->nodes[i];
                        node->value = leaf;
                        node->type = 0;
                    }

                } else {

                    int shift = (n - BYTE_SIZE);
                    uint8_t index = (uint8_t) (block.line >> shift);

                    standard_huffman_decode_node* node = &current_decode_tree->nodes[index];

                    if (node->value == NULL) {
                        node->type = 1;
                        node->value = SAFE_CALLOC(sizeof(standard_huffman_decode_tree));
                    }

                    current_decode_tree = node->value;

                }

                n -= BYTE_SIZE;
            }
        }
    }
}


int standard_huffman_decode_tree_decode(standard_huffman_decode_tree* tree,
                                        huffman_stream* stream,
                                        path_cache* cache) {
    NULL_CHECK(tree);
    NULL_CHECK(stream);
    NULL_CHECK(cache);

    cache->line <<= cache->length;
    cache->seen -= cache->length;

    cache->seen += huffman_stream_read_n_bits(stream, &cache->line, cache->length);

    //check if the file has ended
    if (!cache->seen) { return EOF; }

    //only use the last 8 bits
    uint8_t index = (uint8_t) cache->line;
    if (tree->nodes[index].value == NULL) { THROW_ERROR(ILLEGAL_STATE, "Illegal code found"); }

    while (tree->nodes[index].type) {

        tree = tree->nodes[index].value;
        cache->seen = (uint16_t) huffman_stream_read_n_bits(stream, &cache->line, BYTE_SIZE);
        index = (uint8_t) cache->line;
        if (tree->nodes[index].value == NULL) { THROW_ERROR(ILLEGAL_STATE, "Illegal code found"); }

    }
    
    standard_huffman_decode_leaf* leaf = (standard_huffman_decode_leaf*) tree->nodes[index].value;
    cache->length = (uint16_t) leaf->length;

    return leaf->c;
}

void standard_huffman_decode_tree_free(standard_huffman_decode_tree* tree) {
    NULL_CHECK(tree);

    int index = 0;

    while (index < BYTE_RANGE) {
        void* value = tree->nodes[index].value;

        if (value != NULL) {

            if (tree->nodes[index].type) {

                //recursively delete the nested tree
                standard_huffman_decode_tree_free(value);
            } else {

                //skip all nodes that have a pointer to the same value
                while (index < BYTE_RANGE - 1 && tree->nodes[index + 1].value == value ) { index++; }
            }

            free(value);
        }

        index++;
    }
}
