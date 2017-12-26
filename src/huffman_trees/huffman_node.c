//
// Created by tobiah on 12.10.17.
//
#include <stdlib.h>

#include "../data_structures/static_queue.h"
#include "../util/memory_allocations.h"

huffman_node* huffman_node_create(uint8_t value, uint64_t weight) {
    huffman_node* node = SAFE_MALLOC(sizeof(huffman_node));

    node->value = value;
    node->weight = weight;

    node->parent = NULL;
    node->left = NULL;
    node->right = NULL;

    return node;
}

huffman_node* huffman_nodes_merge(huffman_node* first, huffman_node* second) {
    huffman_node* parent = huffman_node_create(0, first->weight + second->weight);

    parent->left = first;
    parent->right = second;
    first->parent = parent;
    second->parent = parent;

    return parent;
}

bool huffman_node_is_leaf(huffman_node* node) {
    NULL_CHECK(node);

    return node->left == NULL;
}

bool huffman_node_is_root(huffman_node* node) {
    NULL_CHECK(node);

    return node->parent == NULL;
}

bool huffman_node_is_child(huffman_node* parent, huffman_node* child) {
    NULL_CHECK(parent);
    NULL_CHECK(child);

    return child->parent == parent;
}

huffman_node* huffman_node_brother(huffman_node* node) {

    huffman_node* parent = node->parent;
    if (parent == NULL) { return NULL; }

    return parent->left == node ? parent->right : parent->left;
}

void huffman_node_find_root_path(huffman_node* node, path_cache* cache) {

    cache->length = 0;

    while (!huffman_node_is_root(node)) {

        if (node->parent->right == node) {
            //set bit
            cache->line |= ((uint64_t) 1) << cache->length;
        } else {
            //clear bit
            cache->line &= ~(((uint64_t) 1) << cache->length);
        }

        cache->length++;
        node = node->parent;
    }
}

uint64_t huffman_node_fill_weights(huffman_node* node) {
    if (node != NULL) {
        if (!huffman_node_is_leaf(node)) {
            node->weight = huffman_node_fill_weights(node->left) + huffman_node_fill_weights(node->right);
        }
        return node->weight;
    }
    return 0;
}

void huffman_node_fwrite(huffman_stream* output, huffman_node* node, static_queue_tree_node* queue) {
    if (node != NULL) {
        if (!huffman_node_is_leaf(node)) {
            huffman_stream_write_bit(output, 0);
            huffman_node_fwrite(output, node->left, queue);
            huffman_node_fwrite(output, node->right, queue);
        } else {
            static_queue_push_tree_node(queue, node);
            huffman_stream_write_bit(output, 1);
        }
    }
}

huffman_node* huffman_node_fread(huffman_stream* input, static_queue_tree_node* queue) {
    NULL_CHECK(input);
    NULL_CHECK(queue);

    int bit = huffman_stream_read_bit(input);
    if (bit == EOF) { return NULL; }
    if (bit < 0 || bit > 1) { THROW_ERROR(ILLEGAL_STATE, "bit can only be 0 or 1?"); }

    huffman_node* node = huffman_node_create(0, 0);

    if (bit) {
        static_queue_push_tree_node(queue, node);
    } else {
        node->left = huffman_node_fread(input, queue);
        if (node->left == NULL) { THROW_ERROR(ILLEGAL_STATE, "unexpected null node"); }
        node->left->parent = node;

        node->right = huffman_node_fread(input, queue);
        if (node->right == NULL) { THROW_ERROR(ILLEGAL_STATE, "unexpected null node"); }
        node->right->parent = node;
    }

    return node;
}

void huffman_node_print(huffman_node* node, size_t tabs) {

    if (node != NULL) {
        for (size_t i = 0; i < tabs; ++i) {
            fprintf(stderr, "\t");
        }
        fprintf(stderr, "VALUE=%c, WEIGHT=%lu, ORDER_NR=%d\n", node->value, (unsigned long) node->weight,
                node->order_number);

        if (node->left != NULL) {
            huffman_node_print(node->left, tabs + 1);
        }

        if (node->right != NULL) {
            huffman_node_print(node->right, tabs + 1);
        }
    }
}


int huffman_node_cmp(huffman_node* first, huffman_node* second) {
    NULL_CHECK(first);
    NULL_CHECK(second);

    if (first->weight > second->weight) {
        return 1;
    }
    if (second->weight > first->weight) {
        return -1;
    }
    return 0;
}

int huffman_node_cmp_qsort(const void* first, const void* second) {
    NULL_CHECK(first);
    NULL_CHECK(second);

    huffman_node* f = *((huffman_node**) first);
    huffman_node* s = *((huffman_node**) second);

    if (f->weight > s->weight) {
        return 1;
    }
    if (s->weight > f->weight) {
        return -1;
    }
    return 0;
}

void huffman_node_free(huffman_node* node) {

    if (node != NULL) {
        if (node->left != NULL) {
            huffman_node_free(node->left);
        }
        if (node->right != NULL) {
            huffman_node_free(node->right);
        }
        free(node);
    }
}





