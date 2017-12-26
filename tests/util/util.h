//
// Created by tobiah on 05/10/17.
//


#ifndef HUFFMAN_PROJECT_UTIL_H
#define HUFFMAN_PROJECT_UTIL_H

#include <stdlib.h>
#include <stdio.h>
#include <setjmp.h>
#include "../../src/util/terminal_colors.h"
#define MESSAGE_SIZE 1024

#define assert_true(test) do {                                                         \
                                if (!(test))                                           \
                                    return failed(__func__, __LINE__, #test);          \
                           } while (0)                                                 \

#define fail() assert_true(0)
#define success() assert_true(1)

#define test(test) do {                                                                \
                        total++;                                                       \
                        char *message = test();                                        \
                        if (message){                                                  \
                            fprintf(stderr, RED "[FAILED] " RESET "%s\n", message);     \
                        } else {                                                       \
                            fprintf(stderr, "%s\n", (GREEN "[SUCCESS] " RESET #test)); \
                            succeeded++;                                               \
                        }                                                              \
                    } while (0)

#define THROW
extern int succeeded;
extern int total;
extern jmp_buf buf;

char* failed(const char* function, int line, const char* code);
void test_info();

int cmp_file(FILE* first, FILE* second);

#endif //HUFFMAN_PROJECT_UTIL_H
