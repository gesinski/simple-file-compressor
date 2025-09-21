#include "huffman-encoding.h"

typedef struct {
    unsigned char sign;
    long occurrances;
} Data;

typedef struct Node {
    Data data;
    struct Node *left;
    struct Node *right;
} Node;

typedef struct {
    Node **characters;
    long size;
} Priority_queue;

void swap(Node *a, Node *b) {
    Node temp = *a;
    *a = *b;
    *b = temp;
}

void heapify_up(Priority_queue *pq, int index) {
    if (index && pq->characters[(index - 1) / 2]->data.occurrances > pq->characters[index]->data.occurrances) {
        swap(&pq->characters[(index - 1) / 2], &pq->characters[index]);
        heapify_up(pq, (index - 1) / 2);
    }
}

Node *extract_min(Priority_queue *pq) {
    if (!pq->size) {
        printf("Priority queue is empty\n");
        return;
    }
    pq->size--;
    return pq->characters[pq->size];
}

long huffman_encode(long buffer_length_rle, unsigned char *buffer_rle, unsigned char *compressed_buffer) {
    long compressed_buffer_length = 0;

    //priority Queue 
    Priority_queue *pq = malloc(sizeof(Priority_queue));
    pq->characters = malloc(buffer_length_rle * sizeof(Node*));
    pq->size = 0;

    for (long i = 0; i < buffer_length_rle; i++) {
        int repetition = 0;
        for (long j = 0; j < pq->size; j++) {
            if (pq->characters[j]->data.sign == buffer_rle[i]) {
                pq->characters[j]->data.occurrances++;
                heapify_up(pq, j);
                repetition = 1;
                break;
            }
        }
        if (!repetition) {
            Node *new_node;
            new_node->data.sign = buffer_rle[i];
            new_node->data.occurrances = 1;

            pq->characters[pq->size] = new_node;
            pq->size++;
        }
    }

    //binary tree   
    while (pq->size > 1) {
        Node *left = extract_min(pq);
        Node *right = extract_min(pq);

        Node *parent = malloc(sizeof(Node));
        parent->data.sign = '\0';
        parent->data.occurrances = left->data.occurrances + right->data.occurrances;
        parent->left = left;
        parent->right = right;

        pq->characters[pq->size] = parent;
        heapify_up(pq, pq->size);
        pq->size++;
    }
    Node *root = pq->characters[0];

    //binary tree to dictionary

    //buffer rle + dictionary = compressed_buffer

    free(pq->characters);
    free(pq);
    return compressed_buffer_length;
}