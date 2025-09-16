#include <stdio.h>
#include <stdlib.h>
#include "rle.h"
#include "rld.h"

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

    rle(buffer, fcompressed, fileSize);

    fclose(fcompressed);
    free(buffer);
    return 0;
}