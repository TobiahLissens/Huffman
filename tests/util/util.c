//
// Created by tobiah on 16.10.17.
//

#include "util.h"

int succeeded = 0;
int total = 0;
jmp_buf buf;

static char message_string[MESSAGE_SIZE];

char *failed(const char *func, int line, const char *code) {
    snprintf(message_string, MESSAGE_SIZE, "%s (assertion: %s @ line: %d)", func, code, line);
    message_string[MESSAGE_SIZE - 1] = '\0';
    return message_string;
}

void test_info() {
    fprintf(stderr, BLUE "tests_succeeded: " RESET "%d\n" BLUE  "test_amount: " RESET"%d\n", succeeded, total);
}

int cmp_file(FILE *first, FILE *second) {
    int first_c = 0, second_c = 0;
    while ((first_c != EOF) && (second_c != EOF) && (first_c == second_c)) {
        first_c = getc(first);
        second_c = getc(second);
    }
    if (first_c == second_c){
        return 1;
    }
    return 0;
}