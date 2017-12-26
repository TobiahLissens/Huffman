//
// Created by tobiah on 03/10/17.
//

#ifndef HUFFMAN_IO_OPERATIONS_H
#define HUFFMAN_IO_OPERATIONS_H

#include "bit_twiddling.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

//macro pad commands
#define PAD (-1)
#define PAD_N_ZERO (-2)

//should be larger than 3 or equal
#define BUFFER_BYTE_SIZE  4096

typedef struct huffman_stream {

    //pointer to the file_stream
    FILE* file_stream;

    //amount of bits left to use
    size_t bit_index;

    //current length in the buffer
    size_t index;

    //amount of bytes read
    size_t bytes_read;

    //signaling flag for different las block signaling
    bool flag;

    //the stream buffer used for fread and fwrite.
    //note :
    //When using uint64_t instead of uint8_t the speed of some algorithms could double
    //This is because most computers are optimised for 64bits
    //I opted not to do this implementation as it would make the io_operations code a lot less readable (endianness conversion etc)
    //But it is a cool observation and surely worth a mention.
    uint8_t byte_buffer[BUFFER_BYTE_SIZE];

} huffman_stream;

/**
 * encapsulate a FILE in a huffman_stream this enables it to write out byte per byte.
 * @param file file to encapsulate in a huffman_stream
 * @return huffman_stream
 */
void huffman_stream_init(FILE* file, huffman_stream* stream);

/**
 * rewind the stream
 * @param stream pointer to the stream
 */
void huffman_stream_rewind(huffman_stream* stream);

/**
 * reset the stream bit index and update the stream index
 * @param stream pointer to the stream
 */
void huffman_stream_reset_bit_index(huffman_stream* stream);

/**
 * reset the stream index and flush
 * @param stream pointer to the stream
 */
void huffman_stream_reset_index_write(huffman_stream* stream);

/**
 * reset the stream index and read
 * @param stream pointer to the stream
 */
void huffman_stream_reset_index_read(huffman_stream* stream);

/**
 * checks how many bits are still valid
 * @param stream the huffman stream
 * @return the amount of bits that are still valid for read operations <= 8
 */
int huffman_stream_check_bits_valid(huffman_stream* stream);

/**
 * check if the stream is EOF
 * @param stream pointer to the stream
 * @return true if EOF else false
 */
bool huffman_stream_EOF(huffman_stream* stream);

/**
 * read a byte from the huffman_stream
 * @param stream pointer to the stream
 * @return byte if not EOF
 */
int huffman_stream_read_byte(huffman_stream* stream);

/**
 * read a bit from the huffman_stream.
 * @param stream pointer to the stream
 * @return bit if not EOF
 */
int huffman_stream_read_bit(huffman_stream* stream);

/**
 * read n bits from the stream
 * @return amount of bits read from the stream
 */
int huffman_stream_read_n_bits(huffman_stream* stream, uint64_t* buffer, uint16_t n);

/**
 * write byte to the stream
 * @param stream pointer to the stream
 * @param byte byte to write to the stream
 */
void huffman_stream_write_byte(huffman_stream* stream, int byte);

/**
 * write a bit to the stream
 * @param stream pointer to the stream
 * @param bit bit to write to the stream
 */
void huffman_stream_write_bit(huffman_stream* stream, int bit);

/**
 * write out n bits to the stream from left to right.
 * @param stream pointer to the stream
 * @param bits the length of the bits
 * @param n the amount of bits to write
 */
void huffman_stream_write_n_bits(huffman_stream* stream, uint64_t bits, uint16_t n);

/**
 * padd the stream to a multiple of 8 bits
 * @param stream pointer to the stream
 * @param keycode -1 just padd to a multiple of 8, -2 padd to a multiple of 8 and write out how many of the bits are valid.
 */
void huffman_stream_pad_bits(huffman_stream* stream, int keycode);

/**
 * skips the remaining bits in a byte
 * @param stream pointer to the stream
 */
void huffman_stream_unpad_bits(huffman_stream* stream);

/**
 * write out buffered bytes.
 * @param stream pointer to the stream
 */
void huffman_stream_flush(huffman_stream* stream);

/**
 * close the huffman_stream
 * @param stream pointer to the stream to close
 */
void huffman_stream_close(huffman_stream* stream);

#endif //HUFFMAN_IO_OPERATIONS_H
