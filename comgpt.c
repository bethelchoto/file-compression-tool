#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_FILENAME_LENGTH 100

struct huffnode {
    unsigned char character;
    unsigned int freq;
    struct huffnode *left, *right;
};

typedef struct huffnode *NODE;

struct Minheap {
    unsigned short size, capacity;
    NODE *array;
};

typedef struct Minheap *HEAP;

// Function prototypes
NODE newleafnode(unsigned char c, unsigned int f);
NODE newinternalnode(unsigned int f);
void swap(NODE *a, int i, int j);
void heapify(HEAP heap);
NODE mintop(HEAP heap);
void insertnode(HEAP heap, NODE leftchild, NODE rightchild, unsigned int f);
void huffman_tree(HEAP heap);
void assign_codes(NODE root, char *str, char **codes);
void print_huffman_tree(NODE root);

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Invalid command line arguments. Usage: ./out <input file>\n");
        return 1;
    }

    FILE *input = fopen(argv[1], "rb"); // Open the input binary file for reading in binary mode
    if (input == NULL) {
        printf("Error opening input file\n");
        return 2;
    }

    // Read the binary file and calculate frequencies
    unsigned char byte1;
    unsigned int freq[256] = {0}; // Assuming 256 possible byte values

    while (fread(&byte1, sizeof(unsigned char), 1, input)) {
        freq[byte1]++;
    }
    fclose(input);

    // Construct the Huffman tree
    HEAP heap = (HEAP)malloc(sizeof(struct Minheap));
    heap->size = 0;
    heap->capacity = 256; // 256 possible byte values
    heap->array = (NODE *)malloc(heap->capacity * sizeof(NODE));
    heap->array[0] = newinternalnode(0); // Dummy node

    // Initialize the heap with leaf nodes
    for (int i = 0; i < 256; i++) {
        if (freq[i]) {
            heap->size++;
            heap->array[heap->size] = newleafnode((unsigned char)i, freq[i]);
        }
    }

    // Build Huffman tree
    heapify(heap);
    huffman_tree(heap);

    // Assign Huffman codes
    char **codes = (char **)malloc(256 * sizeof(char *));
    for (int i = 0; i < 256; i++) {
        codes[i] = (char *)malloc(256 * sizeof(char));
    }
    assign_codes(heap->array[1], "", codes);

    // Open output file for writing in binary mode
    char out[MAX_FILENAME_LENGTH];
    strncpy(out, argv[1], strlen(argv[1]) - 4); // Remove ".bin" extension
    strcat(out, "-compressed.bin");
    FILE *output = fopen(out, "wb");
    if (output == NULL) {
        printf("Error creating output file\n");
        return 3;
    }

    // Write Huffman tree to the output file
    print_huffman_tree(heap->array[1]); // For demonstration, you may need to adjust the format for file writing

    // Write compressed data to the output file
    input = fopen(argv[1], "rb"); // Open the input binary file again
    unsigned char byte;
    char buffer[256]; // Assuming maximum code length of 256
    int buffer_len = 0;
    while (fread(&byte, sizeof(unsigned char), 1, input)) {
        char *code = codes[byte];
        int code_len = strlen(code);
        for (int i = 0; i < code_len; i++) {
            buffer[buffer_len++] = code[i];
            if (buffer_len == 256) {
                fwrite(buffer, sizeof(char), buffer_len, output);
                buffer_len = 0;
            }
        }
    }
    // Write remaining bits in the buffer
    if (buffer_len > 0) {
        fwrite(buffer, sizeof(char), buffer_len, output);
    }
    fclose(input);
    fclose(output);

    printf("File compressed successfully\n");

    return 0;
}

NODE newleafnode(unsigned char c, unsigned int f) {
    NODE tmp = (NODE)malloc(sizeof(struct huffnode));
    tmp->character = c;
    tmp->freq = f;
    tmp->left = NULL;
    tmp->right = NULL;
    return tmp;
}

NODE newinternalnode(unsigned int f) {
    NODE tmp = (NODE)malloc(sizeof(struct huffnode));
    tmp->character = '\0';
    tmp->freq = f;
    tmp->left = NULL;
    tmp->right = NULL;
    return tmp;
}

void swap(NODE *a, int i, int j) {
    NODE tmp = a[i];
    a[i] = a[j];
    a[j] = tmp;
}

void heapify(HEAP heap) {
    int i, j, k;
    unsigned short n = heap->size;
    NODE *a = heap->array;
    for (i = n / 2; i > 0; i--) {
        if (2 * i + 1 <= n)
            j = a[2 * i]->freq < a[2 * i + 1]->freq ? 2 * i : 2 * i + 1;
        else
            j = 2 * i;

        if (a[j]->freq < a[i]->freq) {
            swap(a, i, j);
            while (j <= n / 2 && (a[j]->freq > a[2 * j]->freq || (2 * j + 1 <= n ? a[j]->freq > a[2 * j + 1]->freq : 0))) {
                if (2 * j + 1 <= n)
                    k = a[2 * j]->freq < a[2 * j + 1]->freq ? 2 * j : 2 * j + 1;
                else
                    k = 2 * j;
                swap(a, k, j);
                j = k;
            }
        }
    }
}

NODE mintop(HEAP heap) {
    NODE top = heap->array[1];
    heap->array[1] = heap->array[heap->size--];
    heapify(heap);
    return top;
}

void insertnode(HEAP heap, NODE leftchild, NODE rightchild, unsigned int f) {
    NODE tmp = (NODE)malloc(sizeof(struct huffnode));
    tmp->character = '\0';
    tmp->freq = f;
    tmp->left = leftchild;
    tmp->right = rightchild;
    heap->array[++heap->size] = tmp;
    heapify(heap);
}

void huffman_tree(HEAP heap) {
    while (heap->size > 1) {
        NODE left_child = mintop(heap);
        NODE right_child = mintop(heap);
        insertnode(heap, left_child, right_child, (left_child->freq + right_child->freq));
    }
}

void assign_codes(NODE root, char *str, char **codes) {
    char *s1 = (char *)malloc(strlen(str) + 2); // +2 for appending '0' and null terminator
    char *s2 = (char *)malloc(strlen(str) + 2); // +2 for appending '1' and null terminator
    if (!root->left && !root->right) {
        codes[root->character] = strdup(str); // Assign Huffman code to character
    } else {
        strcpy(s1, str);
        strcpy(s2, str);
        strcat(s1, "0");
        strcat(s2, "1");
        assign_codes(root->left, s1, codes);
        assign_codes(root->right, s2, codes);
    }
    free(s1);
    free(s2);
}

void print_tree(NODE root, int level) {
    if (root != NULL) {
        print_tree(root->right, level + 1);
        for (int i = 0; i < level; ++i) {
            printf("\t");
        }
        if (root->character != '\0') {
            printf("(%c:%d)\n", root->character, root->freq);
        } else {
            printf("(Internal:%d)\n", root->freq);
        }
        print_tree(root->left, level + 1);
    }
}

void print_huffman_tree(NODE root) {
    printf("Huffman Tree:\n");
    print_tree(root, 0);
}
