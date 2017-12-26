//
// Created by tobiah on 03/10/17.
//

#include "io_operations.h"
#include <string.h>

void huffman_stream_init(FILE* file, huffman_stream* stream) {
    NULL_CHECK(stream);

    stream->file_stream = file;
    stream->bit_index = BYTE_SIZE;
    stream->index = 0;
    stream->bytes_read = 0;
    stream->flag = false;

    memset(stream->byte_buffer, 0, BUFFER_BYTE_SIZE);
}

void huffman_stream_rewind(huffman_stream* stream) {
    NULL_CHECK(stream);

    rewind(stream->file_stream);
    huffman_stream_init(stream->file_stream, stream);
}

void huffman_stream_reset_bit_index(huffman_stream* stream) {
    NULL_CHECK(stream);
    if (stream->bit_index == 0) {
        stream->index++;
        stream->bit_index = BYTE_SIZE;
    }
}

void huffman_stream_reset_index_write(huffman_stream* stream) {
    NULL_CHECK(stream);

    if (stream->index >= BUFFER_BYTE_SIZE && stream->bit_index == BYTE_SIZE) {
        fwrite(stream->byte_buffer, sizeof(*stream->byte_buffer), BUFFER_BYTE_SIZE, stream->file_stream);
        memset(stream->byte_buffer, 0, BUFFER_BYTE_SIZE * sizeof(*stream->byte_buffer));
        stream->index = 0;
    }
}

void huffman_stream_reset_index_read(huffman_stream* stream) {
    NULL_CHECK(stream);

    if (stream->index + 2 >= stream->bytes_read && stream->bit_index == BYTE_SIZE && !stream->flag) {

        //check if the stream has already read something
        size_t to_read = 0;

        //if a block already has been read
        if (stream->bytes_read > 0) {

            //shift the last 2 bytes to the frond
            stream->byte_buffer[0] = stream->byte_buffer[stream->bytes_read - 2];
            stream->byte_buffer[1] = stream->byte_buffer[stream->bytes_read - 1];

            to_read = 2;
        }

        stream->bytes_read =
                fread(stream->byte_buffer + to_read, sizeof(*stream->byte_buffer), BUFFER_BYTE_SIZE - to_read,
                      stream->file_stream);


        stream->bytes_read += to_read;

        if (stream->bytes_read < BUFFER_BYTE_SIZE) { stream->flag = true; }

        stream->index = 0;
    }
}

int huffman_stream_check_bits_valid(huffman_stream* stream) {
    NULL_CHECK(stream);

    if (stream->bytes_read != BUFFER_BYTE_SIZE &&
        stream->index + 2 >= stream->bytes_read &&
        feof(stream->file_stream)) {
        if (stream->index + 1 >= stream->bytes_read) { return 0; }
        return (int) (stream->byte_buffer[stream->bytes_read - 1] - (BYTE_SIZE - stream->bit_index));
    }

    return BYTE_SIZE;
}

bool huffman_stream_EOF(huffman_stream* stream) {
    NULL_CHECK(stream);

    return stream->flag && stream->index >= stream->bytes_read;
}

int huffman_stream_read_byte(huffman_stream* stream) {
    NULL_CHECK(stream);

    huffman_stream_reset_index_read(stream);

    if (huffman_stream_EOF(stream)) { return EOF; }

    return stream->byte_buffer[stream->index++];
}

int huffman_stream_read_bit(huffman_stream* stream) {
    NULL_CHECK(stream);

    if (!huffman_stream_check_bits_valid(stream)) { return EOF; }

    huffman_stream_reset_bit_index(stream);
    huffman_stream_reset_index_read(stream);

    if (huffman_stream_EOF(stream)) { return EOF; }

    return (stream->byte_buffer[stream->index] >> (--stream->bit_index)) & 1;
}

int huffman_stream_read_n_bits(huffman_stream* stream, uint64_t* buffer, uint16_t n) {
    NULL_CHECK(stream);

    size_t j;
    uint16_t size = n;

    //n will never be 0 so 64 >> should not be a problem
    *buffer &= ~(((uint64_t) -1) >> (64 - n));

    while (size > 0) {

        huffman_stream_reset_bit_index(stream);
        huffman_stream_reset_index_read(stream);

        int valid = huffman_stream_check_bits_valid(stream);
        if (valid <= 0) { return n - size; } //return the amount of encoded bytes

        j = stream->bit_index < size ? stream->bit_index : size; //number of bits wanted
        j = valid < j ? (size_t) valid : j; //check if the bits are still available from the file

        size -= j;
        stream->bit_index -= j;

        *buffer |= ((uint64_t) stream->byte_buffer[stream->index] >> stream->bit_index) << (size);

    }

    return n;
}

void huffman_stream_write_byte(huffman_stream* stream, int byte) {
    NULL_CHECK(stream);

    huffman_stream_reset_index_write(stream);
    stream->byte_buffer[stream->index++] = (uint8_t) byte;
    stream->bit_index = BYTE_SIZE;
}

void huffman_stream_write_bit(huffman_stream* stream, int bit) {
    NULL_CHECK(stream);

    huffman_stream_reset_bit_index(stream);
    huffman_stream_reset_index_write(stream);
    stream->byte_buffer[stream->index] |= bit << (--stream->bit_index);
}

void huffman_stream_write_n_bits(huffman_stream* stream, uint64_t bits, uint16_t n) {
    NULL_CHECK(stream);

    while (n > 0) {
        huffman_stream_reset_bit_index(stream);
        huffman_stream_reset_index_write(stream);

        //find how many bits we can write to the byte
        size_t j = stream->bit_index < n ? stream->bit_index : n;

        //increase the bit length and decrease the
        stream->bit_index -= j;
        n -= j;

        stream->byte_buffer[stream->index] |=
                (get_uint8_mask(j) & ((uint8_t) (bits >> n))) << (stream->bit_index);
    }
}

void huffman_stream_pad_bits(huffman_stream* stream, int keycode) {
    NULL_CHECK(stream);
    //number of bits used.
    int bits_used = (int) (BYTE_SIZE - stream->bit_index);

    //pad the rest of the bits. Is this really needed
    while (stream->bit_index < BYTE_SIZE && stream->bit_index > 0) {
        stream->byte_buffer[stream->index] &= ~(((uint64_t) 1) << --stream->bit_index);
    }

    if (stream->bit_index == 0) {

        stream->index++;
        stream->bit_index = BYTE_SIZE;

        if (keycode == PAD_N_ZERO) {
            huffman_stream_write_byte(stream, bits_used == 0 ? BYTE_SIZE : bits_used);
        }
    }
}

void huffman_stream_unpad_bits(huffman_stream* stream) {
    NULL_CHECK(stream);

    if (stream->bit_index != BYTE_SIZE) {
        stream->index++;
        stream->bit_index = BYTE_SIZE;
    }
}

void huffman_stream_flush(huffman_stream* stream) {
    NULL_CHECK(stream);

    fwrite(stream->byte_buffer, sizeof(*stream->byte_buffer), stream->index, stream->file_stream);
    fflush(stream->file_stream);
}

void huffman_stream_close(huffman_stream* stream) {
    NULL_CHECK(stream);
    
    huffman_stream_flush(stream);
}







