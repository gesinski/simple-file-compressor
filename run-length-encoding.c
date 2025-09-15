#include <rle.h>

void rle(unsigned char *buffer, FILE *fcompressed, long fileSize) {
    long repeat_count = 0;
    char current_letter;
    unsigned char *compressed_buffer = (unsigned char*) malloc(fileSize);
    long compressed_buffer_length = 0;

    for(long i = 0; i < fileSize; i++) {
        current_letter = buffer[i];
        while(buffer[i] == current_letter) {
            repeat_count++;
            if(i < fileSize)
                i++;
            else 
                break;
        }
        compressed_buffer_length += sprintf(
            (char *)&compressed_buffer[compressed_buffer_length],
            "%l%c",
            repeat_count,
            current_letter
        );
        int count_size = sprintf(
            (char *)&compressed_buffer[compressed_buffer_length],
            "%d%c",
            repeat_count,
            current_letter
        );
        compressed_buffer_length += count_size;
    }
    fwrite(compressed_buffer, 1, fileSize, fcompressed);
    free(compressed_buffer);
} 
