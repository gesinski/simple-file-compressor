#include <stdio.h>
#include <stdlib.h>
#include "rle.h"
#include "rld.h"
#include "huffman-encoding.h"

int main(int argc, char* argv[]) {
    FILE *file, *fcompressed;
    unsigned char *buffer; 
    long fileSize;
    size_t bytesRead;

    char fcompressed_name[1024]; 
    snprintf(fcompressed_name, sizeof(fcompressed_name), "%s.shrink", argv[1]);

    file = fopen(argv[1], "rb");
    if (!file) {
        perror("Can't open given file");
        return 1;
    }

    fseek(file, 0, SEEK_END);
    fileSize = ftell(file);
    rewind(file);

    buffer = (unsigned char*) malloc(fileSize);
    if (!buffer) {
        perror("No memory");
        fclose(file);
        return 1;
    }

    bytesRead = fread(buffer, 1, fileSize, file);
    if (bytesRead != fileSize) {
        perror("Error reading file");
        free(buffer);
        fclose(file);
        return 1;
    }
    fclose(file);

    fcompressed = fopen(fcompressed_name, "wb");
    if (!fcompressed) {
        perror("Can't open out file");
        free(buffer);
        return 1;
    }

    unsigned char *buffer_rle = (unsigned char*) malloc(fileSize);
    long buffer_length_rle = rle(buffer, buffer_rle, fileSize);
    unsigned char *buffer_huffman = (unsigned char*) malloc(buffer_length_rle);
    long compressed_buffer_length = huffman_encode(buffer_length_rle, buffer_rle, buffer_huffman);
    //fwrite(compressed_buffer, 1, compressed_buffer_length, fcompressed);
    //free(compressed_buffer);

    fclose(fcompressed);
    free(buffer);
    free(buffer_rle);
    return 0;
}