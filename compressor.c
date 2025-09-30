#include <stdio.h>
#include <stdlib.h>
#include "rle.h"
#include "rld.h"
#include "huffman-encoding.h"
#include "huffman-decoding.h"

int main(int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <file> <shrink|unshrink>\n", argv[0]);
        return 1;
    }

    char *input_file = argv[1];
    char *mode = argv[2];

    FILE *file;
    unsigned char *buffer;
    long file_size;
    size_t bytes_read;

    if (strcmp(mode, "shrink") == 0) {
        file = fopen(input_file, "rb");
        if (!file) {
            perror("Can't open input file");
            return 1;
        }

        fseek(file, 0, SEEK_END);
        file_size = ftell(file);
        rewind(file);

        buffer = malloc(file_size);
        if (!buffer) {
            perror("malloc");
            fclose(file);
            return 1;
        }

        bytes_read = fread(buffer, 1, file_size, file);
        fclose(file);

        if (bytes_read != file_size) {
            perror("Error reading file");
            free(buffer);
            return 1;
        }

        // prepare output file
        char output_file[1024];
        snprintf(output_file, sizeof(output_file), "%s.shrink", input_file);
        FILE *fcompressed = fopen(output_file, "wb");
        if (!fcompressed) {
            perror("Can't open output file");
            free(buffer);
            return 1;
        }

        // Run RLE
        unsigned char *buffer_rle = malloc(file_size);
        long buffer_length_rle = rle(buffer, buffer_rle, file_size);

        // Run Huffman encoding
        unsigned char *buffer_compressed = malloc(buffer_length_rle * 16);
        long compressed_buffer_length = huffman_encode(buffer_length_rle, buffer_rle, buffer_compressed, fcompressed);

        fputc('#', fcompressed); // marker for end of tree
        fwrite(buffer_compressed, 1, compressed_buffer_length, fcompressed);
        fclose(fcompressed);

        printf("File compressed: %s\n", output_file);

        free(buffer);
        free(buffer_rle);
        free(buffer_compressed);
    }
    else if (strcmp(mode, "unshrink") == 0) {
        size_t len = strlen(input_file);
        if (len < 7 || strcmp(input_file + len - 7, ".shrink") != 0) {
            fprintf(stderr, "Error: file to unshrink must have .shrink extension\n");
            return 1;
        }

        file = fopen(input_file, "rb");
        if (!file) {
            perror("Can't open input file");
            return 1;
        }

        fseek(file, 0, SEEK_END);
        file_size = ftell(file);
        rewind(file);

        unsigned char *compressed_buffer = malloc(file_size);
        if (!compressed_buffer) {
            perror("malloc");
            fclose(file);
            return 1;
        }

        bytes_read = fread(compressed_buffer, 1, file_size, file);
        fclose(file);

        if (bytes_read != file_size) {
            fprintf(stderr, "Error: expected %ld bytes, got %zu\n", file_size, bytes_read);
            free(compressed_buffer);
            return 1;
        }

        // Huffman decode
        unsigned char *buffer_decompressed = (unsigned char *)malloc(file_size);
        long huffman_decompressed_length = huffman_decode(file_size, compressed_buffer, &buffer_decompressed);

        // RLD decode
        unsigned char *final_buffer = NULL;
        long decompressed_length = rld(buffer_decompressed, &final_buffer, huffman_decompressed_length);

        // prepare output name
        char output_file[1024];
        snprintf(output_file, sizeof(output_file), "%s.unshrink", input_file);

        FILE *funcompressed = fopen(output_file, "wb");
        if (!funcompressed) {
            perror("Can't open output file");
            free(compressed_buffer);
            free(buffer_decompressed);
            free(final_buffer);
            return 1;
        }

        fwrite(final_buffer, 1, decompressed_length, funcompressed);
        fclose(funcompressed);

        printf("File decompressed: %s\n", output_file);

        free(compressed_buffer);
        free(buffer_decompressed);
        free(final_buffer);
    }
    else {
        fprintf(stderr, "Unknown mode: %s (use shrink or unshrink)\n", mode);
        return 1;
    }

    return 0;
}
