#include "rld.h"

long rld(unsigned char *after_huffman_buffer, unsigned char *final_buffer, long after_huffman_buffer_length) {
    long file_size = 0;

    for (long i = 0; i < after_huffman_buffer_length; i++) {
        long occurances = 0;
        unsigned char sign;

        while (i < after_huffman_buffer_length && isdigit(after_huffman_buffer[i])) {
            occurances += (int)after_huffman_buffer[i];
            i++;
        }
        sign = after_huffman_buffer[i];

        do {
            final_buffer[file_size] = sign;
            file_size += 1;
        } while (occurances--);
    }

    return file_size;
}