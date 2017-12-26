//
// Created by tobiah on 17/10/17.
//

#include "test_static_queue.h"
#include <stddef.h>
#include "../util/util.h"
#include "../../src/data_structures/static_queue.h"

void test_all_static_queue() {
    test(test_init_static_queue);
    test(test_static_queue_push);
    test(test_static_queue_peek);
    test(test_static_queue_pop);
    test(test_static_queue_size);
    test(test_static_queue_get);
    test(test_static_queue_reset);
}

char *test_init_static_queue() {
    int proto_queue[1000];
    static_queue_int queue;
    static_queue_init_int(&queue, proto_queue, 1000);
    assert_true(queue.capacity == 1000);
    assert_true(queue.position_back == 0);
    assert_true(queue.position_front == 0);
    return NULL;
}

char *test_static_queue_push() {
    int proto_queue[1000];

    static_queue_int queue;
    static_queue_init_int(&queue, proto_queue, 1000);

    for (int i = 0; i < 1000; ++i) {
        static_queue_push_int(&queue, i);
    }

    for (int j = 0; j < 100; ++j) {
        assert_true(queue.values[j] == j);
    }
    return NULL;
}

char *test_static_queue_pop() {
    int proto_queue[1000];

    static_queue_int queue;
    static_queue_init_int(&queue, proto_queue, 1000);

    for (int i = 0; i < 1000; ++i) {
        static_queue_push_int(&queue, i);
    }

    for (int j = 0; j < 1000; ++j) {
        assert_true(static_queue_pop_int(&queue) == j);
    }

    for (int k = 0; k < 1000; ++k) {
        static_queue_push_int(&queue, k);
        assert_true(k == static_queue_pop_int(&queue));
    }
    return NULL;
}

char *test_static_queue_peek() {

    int proto_queue[1000];

    static_queue_int queue;
    static_queue_init_int(&queue, proto_queue, 1000);

    for (int i = 0; i < 1000; ++i) {
        static_queue_push_int(&queue, i);
    }
    for (int k = 0; k < 1000; ++k) {
        for (int j = 0; j < 1000; ++j) {
            assert_true(static_queue_peek_int(&queue) == k);
        }
        assert_true(static_queue_pop_int(&queue) == k);
    }
    return NULL;
}

char *test_static_queue_size() {
    int proto_queue[1000];

    static_queue_int queue;
    static_queue_init_int(&queue, proto_queue, 1000);
    for (int i = 0; i < 100000; ++i) {
        static_queue_push_int(&queue, i);
        static_queue_pop_int(&queue);
        assert_true(static_queue_size_int(&queue) == 0);
    }

    for (int i = 0; i < 1000; ++i) {
        static_queue_push_int(&queue, i);
        assert_true(static_queue_size_int(&queue) == (i + 1));
    }

    return NULL;
}

char *test_static_queue_get() {

    int proto_queue[1000];

    static_queue_int queue;
    static_queue_init_int(&queue, proto_queue, 1000);
    for (int i = 0; i < 1000; ++i) {
        static_queue_push_int(&queue, i);
    }

    for (int j = 0; j < 1000; ++j) {
        assert_true(static_queue_get_int(&queue, j) == j);
    }

    for (int i = 0; i < 1000; ++i) {
        static_queue_pop_int(&queue);
        static_queue_push_int(&queue, i);
    }

    for (int j = 0; j < 1000; ++j) {
        assert_true(static_queue_get_int(&queue, j) == j);
    }
    return NULL;
}


char *test_static_queue_reset() {
    int proto_queue[1000];

    static_queue_int queue;
    static_queue_init_int(&queue, proto_queue, 1000);

    for (int i = 0; i < 1000; ++i) {
        static_queue_push_int(&queue, i);
    }

    for (int j = 0; j < 100; ++j) {
        assert_true(queue.values[j] == j);
    }

    static_queue_reset_int(&queue);

    assert_true(queue.capacity == 1000);
    assert_true(queue.position_back == 0);
    assert_true(queue.position_front == 0);
    return NULL;
}
