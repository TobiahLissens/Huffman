//
// Created by tobiah on 05/10/17.
//

#include "../util/memory_allocations.h"
#include "static_queue.h"

#define DEFINE_STATIC_QUEUE(T, extension)                                                                        \
    void static_queue_init_##extension(static_queue_##extension *queue, T *array, size_t capacity) {             \
        NULL_CHECK(queue);                                                                                       \
        queue->capacity = (capacity == 0) ? BYTE_RANGE : capacity;                                               \
        queue->values = (array != NULL) ? array : SAFE_MALLOC(sizeof(T) * queue->capacity);                      \
        queue->size = 0;                                                                                         \
        queue->position_front = 0;                                                                               \
        queue->position_back = 0;                                                                                \
    }                                                                                                            \
                                                                                                                 \
    void static_queue_push_##extension(static_queue_##extension *queue, T value) {                               \
        NULL_CHECK(queue);                                                                                       \
        if (queue->position_back >= queue->capacity) queue->position_back = 0;                                   \
        queue->size++;                                                                                           \
        queue->values[queue->position_back++] = value;                                                           \
    }                                                                                                            \
                                                                                                                 \
    T static_queue_get_##extension(static_queue_##extension *queue, int index){                                  \
        NULL_CHECK(queue);                                                                                       \
        if (index >= static_queue_size_##extension(queue)) THROW_ERROR(ILLEGAL_STATE, "length out of range.");    \
        int shift_index = queue->position_front + index;                                                         \
        if (shift_index >= queue->capacity) shift_index -= queue->capacity;                                      \
        return queue->values[shift_index];                                                                       \
    }                                                                                                            \
                                                                                                                 \
    T static_queue_pop_##extension(static_queue_##extension *queue) {                                            \
        NULL_CHECK(queue);                                                                                       \
        if (!static_queue_size_##extension(queue)) THROW_ERROR(ILLEGAL_STATE, "queue cannot be empty");          \
        if (queue->position_front >= queue->capacity) queue->position_front = 0;                                 \
        queue->size--;                                                                                           \
        return queue->values[queue->position_front++];                                                           \
    }                                                                                                            \
                                                                                                                 \
    bool static_queue_is_full_##extension(static_queue_##extension *queue){                                      \
        NULL_CHECK(queue);                                                                                       \
        return queue->capacity == static_queue_size_##extension(queue);                                          \
    }                                                                                                            \
                                                                                                                 \
    size_t static_queue_size_##extension(static_queue_##extension *queue) {                                      \
        NULL_CHECK(queue);                                                                                       \
        return queue->size;                                                                                      \
    }                                                                                                            \
                                                                                                                 \
    T static_queue_peek_##extension(static_queue_##extension *queue) {                                           \
        NULL_CHECK(queue);                                                                                       \
        if (!static_queue_size_##extension(queue)) THROW_ERROR(ILLEGAL_STATE, "queue cannot be empty");          \
        return queue->values[queue->position_front];                                                             \
    }                                                                                                            \
                                                                                                                 \
    void static_queue_reset_##extension(static_queue_##extension *queue) {                                       \
        NULL_CHECK(queue);                                                                                       \
        queue->position_front = 0;                                                                               \
        queue->position_back = 0;                                                                                \
        queue->size = 0;                                                                                         \
    }                                                                                                            \
                                                                                                                 \
    void static_queue_free_##extension(static_queue_##extension *queue) {                                        \
        NULL_CHECK(queue);                                                                                       \
        free(queue->values);                                                                                     \
    }                                                                                                            \
                                                                                                                 \
    T static_queue_pop_lowest_##extension(int (*cmp)(T, T),                                                      \
                                            static_queue_##extension *first,                                     \
                                            static_queue_##extension *second) {                                  \
        NULL_CHECK(cmp);                                                                                         \
        NULL_CHECK(first);                                                                                       \
        NULL_CHECK(second);                                                                                      \
        size_t f_size = static_queue_size_##extension(first);                                                    \
        size_t s_size = static_queue_size_##extension(second);                                                   \
        static_queue_##extension *choice = NULL;                                                                 \
        if (f_size < 1 && s_size < 1)                                                                            \
            THROW_ERROR(ILLEGAL_STATE, "The sum of the size of both arrays should be at least 1.");              \
        else if (f_size >= 1 && s_size < 1)                                                                      \
            choice = first;                                                                                      \
        else if (f_size < 1 && s_size >= 1)                                                                      \
            choice = second;                                                                                     \
        else                                                                                                     \
            choice = (cmp(static_queue_peek_##extension(first), static_queue_peek_##extension(second)) < 0)      \
                     ? first : second;                                                                           \
        return static_queue_pop_##extension(choice);                                                             \
    }                                                                                                            \

//define static_queue_int
DEFINE_STATIC_QUEUE(int, int)

//define static_queue_tree_node
DEFINE_STATIC_QUEUE(huffman_node*, tree_node)
