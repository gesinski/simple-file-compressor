#include "huffman-encoding.h"

typedef struct {
    unsigned char sign;
    long occurrances;
} Node;

typedef struct {
    Node *characters;
    long size;
} Priority_queue;

int contains(Priority_queue *pq, unsigned char token) {
    for (long i = 0; i < pq->size; i++) {
        if (pq->characters[i].sign == token) {
            return 1;
        }
    }
    return 0;
}

long huffman_encode(long buffer_length_rle, unsigned char *buffer_rle, unsigned char *compressed_buffer) {
    long compressed_buffer_length = 0;

    Priority_queue *pq = malloc(sizeof(Priority_queue));
    pq->characters = malloc(buffer_length_rle * sizeof(Node));
    pq->size = 0;

    for (long i = 0; i < buffer_length_rle; i++) {
        if (!contains(pq, buffer_rle[i])) {
            Node new_node;
            new_node.sign = buffer_rle[i];
            new_node.occurrances = 1;
            // add new_node to Priority Queue
        } else {
            // increase occurrences of the node
        }
    }

    free(pq->characters);
    free(pq);
    return compressed_buffer_length;
}