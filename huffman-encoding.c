#include "huffman-encoding.h"

void swap(Node **a, Node **b) {
    Node *temp = *a;
    *a = *b;
    *b = temp;
}

void heapify_up(Priority_queue *pq, int index) {
    while (index > 0) {
        int parent = (index - 1) / 2;
        if (pq->characters[parent]->data.occurrances > pq->characters[index]->data.occurrances) {
            swap(&pq->characters[parent], &pq->characters[index]);
            index = parent;
        } else break;
    }
}

void heapify_down(Priority_queue *pq, int index) {
    while (1) {
        int left = 2*index + 1;
        int right = 2*index + 2;
        int smallest = index;

        if (left < pq->size && pq->characters[left]->data.occurrances < pq->characters[smallest]->data.occurrances)
            smallest = left;
        if (right < pq->size && pq->characters[right]->data.occurrances < pq->characters[smallest]->data.occurrances)
            smallest = right;

        if (smallest == index) break;
        swap(&pq->characters[index], &pq->characters[smallest]);
        index = smallest;
    }
}

Node *extract_min(Priority_queue *pq) {
    if (pq->size == 0) return NULL;
    Node *res = pq->characters[0];
    pq->characters[0] = pq->characters[pq->size - 1];
    pq->size--;
    if (pq->size > 0) heapify_down(pq, 0);
    return res;
}

void pq_insert(Priority_queue *pq, Node *n) {
    pq->characters[pq->size] = n;
    heapify_up(pq, pq->size);
    pq->size++;
}

void build_codes(Node* root, char current_code[], unsigned int bits_occupied, unsigned char *codes[]) {
    if (root == NULL) {
        return;
    }

    if (root->left == NULL && root->right == NULL) {
        current_code[bits_occupied] = '\0';
        unsigned char sym = root->data.sign;
        codes[sym] = malloc(bits_occupied + 1);
        if (!codes[sym]) { perror("malloc"); exit(1); }
        strcpy(codes[sym], current_code);
        return;
    }

    current_code[bits_occupied] = '0';
    current_code[bits_occupied + 1] = '\0';
    build_codes(root->left, current_code, bits_occupied + 1, codes);

    current_code[bits_occupied] = '1';
    current_code[bits_occupied + 1] = '\0';
    build_codes(root->right, current_code, bits_occupied + 1, codes);
}

void write_tree(Node *root, FILE *file) {
    if (!root) return;

    if (root->left != NULL || root->right != NULL) {
        fputc('0', file);
        write_tree(root->left, file);
        write_tree(root->right, file);
    } else {
        fputc('1', file);
        fputc(root->data.sign, file);
    }
}

long huffman_encode(long buffer_length_rle, unsigned char *buffer_rle, unsigned char *compressed_buffer, FILE *fcompressed) {
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
            Node *new_node = malloc(sizeof(Node));
            if (!new_node) { perror("malloc"); exit(1); }
            new_node->data.sign = buffer_rle[i];
            new_node->data.occurrances = 1;
            new_node->left = new_node->right = NULL;
            pq_insert(pq, new_node);
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
        pq_insert(pq, parent);
    }
    Node *root = (pq->size == 1) ? extract_min(pq) : NULL;

    //binary codes to dictionary
    unsigned char *huffman_codes[256];
    for (int i = 0; i < 256; i++) {
        huffman_codes[i] = NULL;
    }
    char current_code[256];
    build_codes(root, current_code, 0, huffman_codes);

    //store binary codes
    unsigned int bits_used = 0;
    unsigned char byte = 0;
    for (long buffer_sign_index = 0; buffer_sign_index < buffer_length_rle; buffer_sign_index++) {
        unsigned char current_sign = buffer_rle[buffer_sign_index];
        char *code = huffman_codes[current_sign];
        for (int k = 0; code[k] != '\0'; k++) {
            int bit = (code[k] == '1') ? 1 : 0;
            byte |= (bit << (7 - bits_used));
            bits_used++;
            if (bits_used == 8) {
                compressed_buffer[compressed_buffer_length++] = byte;
                bits_used = 0;
                byte = 0;
            }
        }
    }
    if (bits_used > 0) {
        compressed_buffer[compressed_buffer_length++] = byte;
    }

    //huffman tree write to compressed file
    write_tree(root, fcompressed);

    //debug
    // for (long i = 0; i < compressed_buffer_length; i++) {
    //     for (int b = 7; b >= 0; b--) {
    //         printf("%d", (compressed_buffer[i] >> b) & 1);
    //     }
    //     printf(" ");
    // }
    // printf("\n");

    //free memory
    for (int i = 0; i < 256; i++) {
        if (huffman_codes[i] != NULL) free(huffman_codes[i]);
    }
    free(pq->characters);
    free(pq);
    return compressed_buffer_length;
}