//
// Created by tobiah on 04/10/17.
//

#ifndef HUFFMAN_PROJECT_ERRORS_H
#define HUFFMAN_PROJECT_ERRORS_H


#define ERROR_COUNT 4

#define OUT_OF_MEMORY 1
#define ILLEGAL_STATE 2
#define ILLEGAL_ARGUMENT 3
#define UNKNOWN_CLI_ARGUMENT 4

#define THROW_ERROR(code, message) throw_error(code, __func__, message)

#ifdef DEBUG
#define NULL_CHECK(ptr) null_check(ptr, __func__)
#else
#define NULL_CHECK(ptr)
#endif

#include "stdio.h"
#include "terminal_colors.h"
#include <stdlib.h>
#include <execinfo.h>

#ifdef TEST

#include "../../tests/util/util.h"

#endif

#ifndef TEST

const static char* ERRORS[ERROR_COUNT] = {
        "Out of memory exception.",
        "Illegal state exception.",
        "Illegal argument exception.",
        "The given argument is unknown.",
};

#endif

/**
 * print stacktrace
 */
void static inline print_trace() {
    void* array[10];
    int size;
    char** strings;
    size_t i;

    size = backtrace(array, 10);
    strings = backtrace_symbols(array, size);

    fprintf(stderr, BLUE "        Stack trace: " RESET "Obtained %d stack frames.\n", size);

    for (i = 0; i < size; i++) {
        fprintf(stderr, "        %s\n", strings[i]);
    }
    free(strings);
}

/**
 * throw an error
 * @param code exit code
 * @param function_name name of the calling function (mostly  __func__ macro)
 * @param message extra message containing more info on the error
 */
void static inline throw_error(int code, const char* function_name, const char* message) {
#ifdef TEST
    longjmp(buf, 1); //this makes it possible to catch errors in debug mode
#else
    if (code < 1 || code > ERROR_COUNT) {
        throw_error(ILLEGAL_ARGUMENT, __func__, "code should be between 1 and ERROR_SIZE");
        return;
    }
    fprintf(stderr, RED "[ERROR] " RESET "%s\n", ERRORS[code - 1]);
    if (function_name != NULL) {
        fprintf(stderr, BLUE "        In function: " RESET "%s.\n", function_name);
    }

    if (message != NULL) {
        fprintf(stderr, BLUE "        Message: " RESET "%s.\n", message);
    }

    print_trace();
    exit(code);
#endif
}

void static inline null_check(const void* ptr, const char* function_name) {
    if (ptr == NULL) {
        throw_error(ILLEGAL_ARGUMENT, function_name, "ptr should not be null");
    }
}


#endif //HUFFMAN_PROJECT_ERRORS_H
