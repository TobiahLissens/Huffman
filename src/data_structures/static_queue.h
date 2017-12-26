//
// Created by tobiah on 05/10/17.
//


#ifndef HUFFMAN_PROJECT_QUEUE_H
#define HUFFMAN_PROJECT_QUEUE_H

#include "../huffman_trees/huffman_node.h"

#define DECLARE_STATIC_QUEUE(T, extension)                                                                  \
                                                                                                            \
    typedef struct static_queue_##extension {                                                               \
        size_t position_front;                                                                              \
        size_t position_back;                                                                               \
        size_t size;                                                                                        \
        T *values;                                                                                          \
        size_t capacity;                                                                                    \
    } static_queue_##extension;                                                                             \
                                                                                                            \
    /**
	 * initialize the queue.
	 * @param queue queue to initialize
	 * @param capacity initial queue size
	 */                                                                                                     \
    void static_queue_init_##extension(static_queue_##extension *queue, T *array, size_t capacity);         \
                                                                                                            \
    /**
	 * get the size of the queue
	 * @param queue pointer to the queue
	 * @return size of the queue
	 */                                                                                                     \
    size_t static_queue_size_##extension(static_queue_##extension *queue);                                  \
                                                                                                            \
    /**
     * check if the stack is full
     * @param queue pointer to the queue
     * @ruturn 1 if stack is full else 0
     */                                                                                                     \
    bool static_queue_is_full_##extension(static_queue_##extension *queue);                                 \
                                                                                                            \
    /**
	 * push an element on the queue.
	 * @param queue queue the element is pushed on
	 * @param value element pushed on the queue
	 */                                                                                                     \
    void static_queue_push_##extension(static_queue_##extension *queue, T value);                           \
                                                                                                            \
    /**
     * get the value at a certain index in the queue
     * @param queue pointer to the queue
     * @param index index of the element
     */                                                                                                     \
    T static_queue_get_##extension(static_queue_##extension *queue, int index);                             \
                                                                                                            \
    /**
	 * get the first element of the queue.
	 * @param queue pointer to the queue
	 * @return first element of queue
	 */                                                                                                     \
    T static_queue_peek_##extension(static_queue_##extension *queue);                                       \
                                                                                                            \
    /**
	 * pop an element from the queue.
	 * @param queue pointer to the queue.
	 * @return first element of the queue
	 */                                                                                                     \
    T static_queue_pop_##extension(static_queue_##extension *queue);                                        \
                                                                                                            \
    /**
	 * pops the lowest value from the 2 queue's
	 * @param cmp comparator
	 * @param first pointer to the first static queue
	 * @param second pointer to the second static queue
	 * @return pop the lowest value of the queue
	 */                                                                                                     \
    T static_queue_pop_lowest_##extension(int (*cmp)(T, T),                                                 \
                        static_queue_##extension *first,                                                    \
                        static_queue_##extension *second);                                                  \
                                                                                                            \
    /**
	 * make the queue ready for reuse
	 * @param queue pointer to the queue
	 */                                                                                                     \
    void static_queue_reset_##extension(static_queue_##extension *queue);                                   \
                                                                                                            \
    /**
	 * important only call this when the array is dynamically initialized!!!!
	 * free the allocated memory
	 * @param queue pointer to the queue
	 */                                                                                                     \
    void static_queue_free_##extension(static_queue_##extension *queue);                                    \


//declare static_queue_int
DECLARE_STATIC_QUEUE(huffman_node*, tree_node)

//declare static_queue_tree_node
DECLARE_STATIC_QUEUE(int, int)

#endif //HUFFMAN_PROJECT_QUEUE_H

