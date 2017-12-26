//
// Created by tobiah on 08.10.17.
//

#ifndef HUFFMAN_PROJECT_BIT_TWIDDLING_H
#define HUFFMAN_PROJECT_BIT_TWIDDLING_H

#include "errors.h"
#include "stdlib.h"
#include <stdint.h>
#include <values.h>

//size of an uint8_t integer.
#define BYTE_SIZE 8

//amount of different possibilities 2^8
#define BYTE_RANGE 256

#define get_uint8_mask(i) UINT8_MASK_TABLE[i]

const static uint8_t UINT8_MASK_TABLE[] = {0b0, 0b1, 0b11, 0b111, 0b1111, 0b11111, 0b111111, 0b1111111, 0b11111111};

#endif //HUFFMAN_PROJECT_BIT_TWIDDLING_H
