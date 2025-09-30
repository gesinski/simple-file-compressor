#include "../include/rld.h"
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>

long rld(unsigned char *after_huffman_buffer, unsigned char **final_buffer, long after_huffman_buffer_length) {
    long file_size = 0;
    long capacity = 1024; 

    *final_buffer = malloc(capacity);
    if (!*final_buffer) {
        perror("malloc");
        exit(1);
    }

    for (long i = 0; i < after_huffman_buffer_length; i++) {
        long occurances = 0;
        unsigned char sign;

        while (i < after_huffman_buffer_length && isdigit(after_huffman_buffer[i])) {
            occurances = occurances * 10 + (after_huffman_buffer[i] - '0'); 
            i++;
        }

        if (i >= after_huffman_buffer_length) break;
        sign = after_huffman_buffer[i];
        if (occurances == 0) occurances = 1;

        for (long k = 0; k < occurances; k++) {
            if (file_size >= capacity) {
                capacity *= 2;
                unsigned char *tmp = realloc(*final_buffer, capacity);
                if (!tmp) {
                    perror("realloc");
                    free(*final_buffer);
                    exit(1);
                }
                *final_buffer = tmp;
            }
            (*final_buffer)[file_size++] = sign;
        }
    }

    return file_size;
}
