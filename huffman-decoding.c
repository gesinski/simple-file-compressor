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

long huffman_decode(long buffer_length, unsigned char *compressed_buffer, unsigned char **decompressed_buffer) {
    long idx = 0;
    long decompressed_length = 0;
    long capacity = buffer_length;
    Node *root = read_tree(compressed_buffer, &idx);
    if (compressed_buffer[idx] == '#') {
        idx++;
    }

    Node *current = root;

    while (idx < buffer_length) {
        unsigned char byte = compressed_buffer[idx];
        for (int i = 0; i < 8; i++) {
            int bit = (byte >> (7 - i)) & 1;
            current = bit ? current->right : current->left;

            if (current->left == NULL && current->right == NULL) {
                if (decompressed_length >= capacity) {
                    capacity *= 2;
                    unsigned char *tmp = realloc(*decompressed_buffer, capacity);
                    if (!tmp) {
                        perror("realloc");
                        free(*decompressed_buffer);
                        free_tree(root);
                        exit(1);
                    }
                    *decompressed_buffer = tmp;
                }
                (*decompressed_buffer)[decompressed_length++] = current->sign;
                current = root; 
            }
        }
        idx++;
    }

    free_tree(root);
    return decompressed_length;
}
