//
// Created by tobiah on 05/10/17.
//


#include <signal.h>
#include "test_huffman_algorithms/test_algorithms.h"
#include "test_data_structures/test_all_data_structures.h"
#include "test_util/test_io_operations.h"

int main(int argc, char** argv) {
    test_all_huffman_algorithms();
    test_all_data_structures();
    test_all_io_operations();
    test_all_data_structures();

    test_info();
    return 0;
}