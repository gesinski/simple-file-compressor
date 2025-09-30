#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char sign;
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

long huffman_encode(long, unsigned char *, unsigned char *, FILE *);