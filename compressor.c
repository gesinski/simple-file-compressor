#include <stdio.h>
#include <stdlib.h>
#include "rle.h"
#include "rld.h"
#include "huffman-encoding.h"
#include "huffman-decoding.h"

int main(int argc, char* argv[]) {
    FILE *file, *fcompressed;
    unsigned char *buffer; 
    long file_size;
    size_t bytes_read;

    char fcompressed_name[1024]; 
    snprintf(fcompressed_name, sizeof(fcompressed_name), "%s.shrink", argv[1]);

    file = fopen(argv[1], "rb");
    if (!file) {
        perror("Can't open given file");
        return 1;
    }

    fseek(file, 0, SEEK_END);
    file_size = ftell(file);
    rewind(file);

    buffer = (unsigned char*) malloc(file_size);
    if (!buffer) {
        perror("No memory");
        fclose(file);
        return 1;
    }

    bytes_read = fread(buffer, 1, file_size, file);
    if (bytes_read != file_size) {
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

    unsigned char *buffer_rle = (unsigned char*) malloc(file_size);
    long buffer_length_rle = rle(buffer, buffer_rle, file_size);
    unsigned char *buffer_compressed = (unsigned char*) malloc(buffer_length_rle * 16); 
    long compressed_buffer_length = huffman_encode(buffer_length_rle, buffer_rle, buffer_compressed, fcompressed);
    
    fputc('#', fcompressed); //marker for the end of tree
    fwrite(buffer_compressed, 1, compressed_buffer_length, fcompressed);
    fclose(fcompressed);
    free(buffer);

    FILE *f = fopen(fcompressed_name, "rb");
    if (!f) {
        perror("Can't open file");
        return 1;
    }

    fseek(f, 0, SEEK_END);
    long ffile_size = ftell(f);
    rewind(f);

    unsigned char *compressed_buffer = malloc(ffile_size);
    if (!buffer) {
        perror("malloc");
        fclose(f);
        return 1;
    }

    size_t bytes = fread(compressed_buffer, 1, ffile_size, f);
    fclose(f);

    if (bytes != ffile_size) {
        fprintf(stderr, "Error: expected %ld bytes, got %zu\n", ffile_size, bytes);
        free(compressed_buffer);
        return 1;
    }

    unsigned char *buffer_decompressed = (unsigned char*) malloc(ffile_size);
    long huffman_decompressed_length = huffman_decode(ffile_size, compressed_buffer, &buffer_decompressed);
    unsigned char *decompressed;
    long decompressed_length = rld(buffer_decompressed, huffman_decompressed_length, &decompressed);

    char funcompressed_name[1024];
    snprintf(funcompressed_name, sizeof(funcompressed_name), "%s.unshrink", argv[1]);

    FILE *funcompressed = fopen(funcompressed_name, "wb");
    if (!funcompressed) {
        perror("Can't open unshrink file");
        free(compressed_buffer);
        free(buffer_rle);
        free(buffer_decompressed);
        free(decompressed);
        return 1;
    }
    fwrite(decompressed, 1, decompressed_length, funcompressed);
    fclose(funcompressed);

    free(compressed_buffer);
    free(buffer_rle);
    free(buffer_decompressed);
    free(decompressed);
    return 0;
}