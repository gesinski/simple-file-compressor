#include "huffman-decoding.h"

typedef struct Node {
    unsigned char sign;
    struct Node *left;
    struct Node *right;
} Node;

Node *read_tree(unsigned char *buffer, long *index) {
    if (buffer[*index] == '0') {
        (*index)++; 
        Node *new_node = malloc(sizeof(Node));
        new_node->sign = '\0';
        new_node->left  = read_tree(buffer, index);
        new_node->right = read_tree(buffer, index);
        return new_node;
    } 
    else if (buffer[*index] == '1') {
        (*index)++; 
        unsigned char sym = buffer[*index]; 
        (*index)++; 
        Node *new_node = malloc(sizeof(Node));
        new_node->sign = sym;
        new_node->left = new_node->right = NULL;
        return new_node;
    }
    return NULL;
}

void free_tree(Node *root) {
    if (!root) return;
    free_tree(root->left);
    free_tree(root->right);
    free(root);
}

long huffman_decode(unsigned char *compressed_buffer, unsigned char *decompressed_buffer) {
    long idx = 0;
    Node *root = read_tree(compressed_buffer, &idx);
    
    free_tree(root);
}