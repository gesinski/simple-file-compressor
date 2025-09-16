#include "rle.h"

void rle(unsigned char *buffer, FILE *fcompressed, long fileSize) {
    unsigned char *compressed_buffer = (unsigned char*) malloc(fileSize);
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
    fwrite(compressed_buffer, 1, compressed_buffer_length, fcompressed);
    free(compressed_buffer);
} 
