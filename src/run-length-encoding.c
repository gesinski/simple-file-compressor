#include "../include/rle.h"

long rle(unsigned char *buffer, unsigned char *compressed_buffer, long fileSize) {
    long compressed_buffer_length = 0;

    for(long i = 0; i < fileSize; ) {
        char current_letter = buffer[i];
        long repeat_count = 0;

        while(i < fileSize && buffer[i] == current_letter) {
            repeat_count++;
            i++;
        }
        if (repeat_count > 1) {
            long count_size = sprintf(
            (char *)&compressed_buffer[compressed_buffer_length],
            "%ld%c",
            repeat_count,
            current_letter
            );
            compressed_buffer_length += count_size;
        } else {
            compressed_buffer[compressed_buffer_length++] = current_letter;
        }
    }
    return compressed_buffer_length;
} 
