#include "huffman-encoding.h"

typedef struct {
    unsigned char sign;
    long occurrances;
} Node;

typedef struct {
    Node *characters;
    long size;
} Priority_queue;

void swap(Node *a, Node *b) {
    Node temp = *a;
    *a = *b;
    *b = temp;
}

void heapify_up(Priority_queue *pq, int index) {
    if (index && pq->characters[(index - 1) / 2].occurrances > pq->characters[index].occurrances) {
        swap(&pq->characters[(index - 1) / 2], &pq->characters[index]);
        heapify_up(pq, (index - 1) / 2);
    }
}

typedef struct Leaf {
    Node data;
    struct Leaf *left;
    struct Leaf *right;
} Leaf;

long huffman_encode(long buffer_length_rle, unsigned char *buffer_rle, unsigned char *compressed_buffer) {
    long compressed_buffer_length = 0;

    Priority_queue *pq = malloc(sizeof(Priority_queue));
    pq->characters = malloc(buffer_length_rle * sizeof(Node));
    pq->size = 0;

    for (long i = 0; i < buffer_length_rle; i++) {
        int repetition = 0;
        for (long j = 0; j < pq->size; j++) {
            if (pq->characters[j].sign == buffer_rle[i]) {
                pq->characters[j].occurrances++;
                heapify_up(pq, j);
                repetition = 1;
                break;
            }
        }
        if (!repetition) {
            Node new_node;
            new_node.sign = buffer_rle[i];
            new_node.occurrances = 1;

            pq->characters[pq->size] = new_node;
            pq->size++;
        }
    }

    //binary tree from priority queue


    //binary tree to dictionary

    //buffer rle + dictionary = compressed_buffer

    free(pq->characters);
    free(pq);
    return compressed_buffer_length;
}