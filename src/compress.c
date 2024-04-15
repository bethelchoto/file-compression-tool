#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "error_handling.h"

#define MAX_FILENAME_LENGTH 100
#define SPLIT_STRING "_compressed.bin"

struct huffnode {
    char character;
    unsigned int freq;
    struct huffnode *left, *right;
};

typedef struct huffnode *NODE;

struct Minheap {
    unsigned short size, capacity;
    NODE *array;
};

typedef struct Minheap *HEAP;

struct hashmap_entry {
    char key;
    char *value;
};

typedef struct hashmap_entry *HASHMAP;

NODE newleafnode(char c, int f)
{
    NODE tmp = (NODE)malloc(sizeof(struct huffnode));
    tmp->character = c;
    tmp->freq = f;
    tmp->left = NULL;
    tmp->right = NULL;
    return tmp;
}

NODE newinternalnode(int f)
{
    NODE tmp = (NODE)malloc(sizeof(struct huffnode));
    tmp->character = 0;
    tmp->freq = f;
    tmp->left = NULL;
    tmp->right = NULL;
    return tmp;
}

void swap(NODE *a, int i, int j)
{
    NODE tmp = a[i];
    a[i] = a[j];
    a[j] = tmp;
}

void heapify(HEAP heap)
{
    int i, j, k;
    unsigned short n = heap->size;
    NODE *a = heap->array;
    for (i = n / 2; i > 0; i--)
    {
        if (2 * i + 1 <= n)
            j = a[2 * i]->freq < a[2 * i + 1]->freq ? 2 * i : 2 * i + 1;
        else
            j = 2 * i;

        if (a[j]->freq < a[i]->freq)
        {
            swap(a, i, j);
            while (j <= n / 2 && (a[j]->freq > a[2 * j]->freq || (2 * j + 1 <= n ? a[j]->freq > a[2 * j + 1]->freq : 0)))
            {
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

NODE mintop(HEAP heap)
{
    NODE top = heap->array[1];
    heap->array[1] = heap->array[heap->size--];
    heapify(heap);
    return top;
}

void insertnode(HEAP heap, NODE leftchild, NODE rightchild, int f)
{
    NODE tmp = (NODE)malloc(sizeof(struct huffnode));
    tmp->character = '\0';
    tmp->freq = f;
    tmp->left = leftchild;
    tmp->right = rightchild;
    heap->array[++heap->size] = tmp;
    heapify(heap);
}

void huffman_tree(HEAP heap)
{
    while (heap->size > 1)
    {
        NODE left_child = mintop(heap);
        NODE right_child = mintop(heap);
        insertnode(heap, left_child, right_child, (left_child->freq + right_child->freq));
    }
}

struct hashmap_entry *hashmap;

void assign_codes(NODE root, char* str) {
    char* s1 = (char*)malloc(strlen(str) + 2); // +2 for appending '0' and null terminator
    char* s2 = (char*)malloc(strlen(str) + 2); // +2 for appending '1' and null terminator
    if (!root->left && !root->right) {
        hashmap[(unsigned char)root->character].key = root->character;
        hashmap[(unsigned char)root->character].value = strdup(str); // Using strdup to duplicate string
    } else {
        strcpy(s1, str);
        strcpy(s2, str);
        strcat(s1, "0");
        strcat(s2, "1");
        assign_codes(root->left, s1);
        assign_codes(root->right, s2);
    }
    free(s1); // Freeing allocated memory
    free(s2); // Freeing allocated memory
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



int main(int argc, char *argv[]) {

    // catch error from unpassed file
    if (argc != 2) {
        report_error(ERROR_INVALID_CMD_ARG);
        return 1;
    }
    
    // open file for reading and compression
    FILE *input = fopen(argv[1], "r");
    if (input == NULL) {
        report_error(ERROR_OPEN_INPUT_FILE);
        return 2;
    }

    // print the file type
    char in[MAX_FILENAME_LENGTH];
    strcpy(in, argv[1]);
    char *filetype = strchr(in, '.') + 1;
    printf("%s\n", filetype);

    // get the first section of the filename
    char out[MAX_FILENAME_LENGTH];
    strncpy(out, in, strchr(in, '.') - in);
    strcat(out, SPLIT_STRING);

    // initialize the outfile, open as binary for writing
    FILE *output = fopen(out, "wb");
    if (output == NULL) {
        report_error(ERROR_CREATE_OUTPUT_FILE);
        return 3;
    }

    // initialize buff pointer for reading files
    char buff[1];
    unsigned short num_of_unique_chars = 0;

    // Write file type in the output file
    int ftsize = strlen(filetype);
    char num = ftsize + '0';
    fwrite(&num, 1, 1, output);
    fwrite(filetype, 1, ftsize, output);

    // Holds frequency of each unique character
    // calculate the count of the characters
    int freq[256] = {0};
    int f_size = 0;
    while (fread(buff, 1, 1, input)) {
        if (!freq[buff[0]]) {
            num_of_unique_chars++;
        }
        freq[buff[0]]++;
        f_size++;
    }
    fclose(input);

    // open input file again
    input = fopen(argv[1], "r");

    // initialize Heap for storing the heap tree
    HEAP heap = (HEAP)malloc(sizeof(struct Minheap));
    heap->size = 0;
    heap->capacity = num_of_unique_chars + 1; // Adding 1 to handle 1-based indexing
    heap->array = (NODE*)malloc((heap->capacity) * sizeof(NODE));
    heap->array[0] = newinternalnode(0); // Dummy node

    // initialize heap with all the characters (1-256)
    for (int i = 0; i < 256; i++) {
        if (freq[i]) {
            
            heap->size += 1;
            heap->array[heap->size] = newleafnode(i, freq[i]);
        }
    }
    
    // heapify tree
    heapify(heap);

    //prints to visialize data
    huffman_tree(heap);

    // creating hashmap for codes
    hashmap = (HASHMAP)malloc(256 * sizeof(struct hashmap_entry));
    for (int i = 0; i < num_of_unique_chars; i++) {
        hashmap[(unsigned char)freq[i]].key = freq[i];
    }

    // Creating hashmap for codes
    hashmap = (HASHMAP)malloc(256 * sizeof(struct hashmap_entry));
    for (int i = 0; i < 256; i++) {

        hashmap[i].key = '\0'; // Null character for key
        hashmap[i].value = NULL; // Initialize to NULL
    }

    // add codes to hash map
    assign_codes(heap->array[1], "");

    // Find the size of the input file
    fseek(input, 0, SEEK_END);
    long file_size = ftell(input);
    rewind(input);

    char *file_contents = (char *)malloc((file_size + 1) * sizeof(char));
    file_contents[0] = '\0';
    // Convert file content to binary code
    // By assigning codes to each character
    while (fread(buff, 1, 1, input)) {
        strcat(file_contents, hashmap[buff[0]].value);
    }

    int original_length = strlen(file_contents);
    int padding = (original_length % 8 != 0) ? 8 - (original_length % 8) : 0;

    // Create padding_string with '0's for padding
    char *padding_string = (char *)malloc(padding + 1); // +1 for null terminator
    if (padding_string == NULL) {
        report_error(ERROR_MEMORY_ALLOCATION);
        return 1;
    }
    memset(padding_string, '0', padding); // Initialize with '0's
    padding_string[padding] = '\0'; // Null-terminate the string

    size_t padding_size = original_length + padding;

    char padded_file_contents[padding_size + 1]; // +1 for null terminator

    strcpy(padded_file_contents, padding_string);
    strcat(padded_file_contents, file_contents);

    padded_file_contents[padding_size] = '\0';

    // Write the codes assigned to characters at the start of the file
    for (int i = 0; i < 256; i++) {
        if (hashmap[i].value != NULL){
           
            int code_length = strlen(hashmap[i].value);
            
            // Write the character and its associated code to the output file
            fwrite(&hashmap[i].key, sizeof(char), 1, output);
            fwrite(hashmap[i].value, sizeof(char), code_length, output);
            fwrite("\0", sizeof(char), 1, output);
            
        }
    }

    char null = '\0';
    char pad = padding + '0';
    fwrite(&null, 1, 1, output);
    fwrite(&pad, 1, 1, output);
    fwrite(&null, 1, 1, output);
    fwrite(&null, 1, 1, output);

    char tmp_bin[9];
    int dec, w, i;
    for (int j = 0; padded_file_contents[j]; j = j + 8) {
        strncpy(tmp_bin, &padded_file_contents[j], 8);
        tmp_bin[8] = '\0'; // Null-terminate the binary string
        // Convert to integer equivalent and then character
        dec = strtol(tmp_bin, NULL, 2); // Convert binary string to integer
        buff[0] = dec;
        fwrite(buff, 1, 1, output);
    }
    
    printf("File compressed successfully\n");
    fclose(output);

    return 0;
}

