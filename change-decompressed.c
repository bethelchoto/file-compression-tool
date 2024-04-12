#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define a struct to represent a key-value pair
typedef struct {
    char key[20];  
    char value;
} Hash_map;

void addEntryToMap(Hash_map *decode_map, int *num_of_unique_chars, char *key, char value) {
    if (*num_of_unique_chars >= 16) {
        // Resize the decode_map if needed (for simplicity, this example doesn't include the resizing logic)
        fprintf(stderr, "Decode map is full.\n");
        exit(1);
    }
    strcpy(decode_map[*num_of_unique_chars].key, key);
    decode_map[*num_of_unique_chars].value = value;
    (*num_of_unique_chars)++;

}

// Function to find a key in the decoded map
char findKey(Hash_map *decode_map, int map_size, char *key) {
    for (int i = 0; i < map_size; i++) {
        if (strcmp(decode_map[i].key, key) == 0) {
            return decode_map[i].value;
        }
    }
    return '\0'; // Key not found
}


// Function to convert decimal to binary string
char *dectobin(int decimal) {
    char *s = malloc(9 * sizeof(char)); // 8 bits + 1 for null terminator
    if (s == NULL) {
        fprintf(stderr, "Error allocating memory for binary string.\n");
        exit(1);
    }

    // Starting from the leftmost bit (most significant bit)
    for (int i = 7; i >= 0; i--) {
        s[7 - i] = (decimal & (1 << i)) ? '1' : '0';
    }
    s[8] = '\0'; // Null terminate the string
    return s;
}

int main(int argc, char *argv[]) {

    if (argc != 2) {
        fprintf(stderr, "Invalid cmd line arg. Usage: ./a.out <input file>\n");
        return 1;
    }

    FILE *input = fopen(argv[1], "rb");

    int num_of_unique_chars = 0;
    
    
    if (input == NULL) {
        fprintf(stderr, "Error opening input file\n");
        return 2;
    }

    char in[256];  
    strncpy(in, argv[1], sizeof(in) - 1);
    in[sizeof(in) - 1] = '\0';  
    char *out = malloc(strlen(in) + 16); 
     
    if (out == NULL) {
        fprintf(stderr, "Error allocating memory for output filename.\n");
        fclose(input);
        return 3;
    }

    // Find the position of "-compressed" in the input filename
    char *pos = strstr(argv[1], "-compressed");
    if (pos == NULL) {
        fprintf(stderr, "Error: Input file name does not contain \"-compressed\"\n");
        fclose(input);
        return 1;
    }

    // Calculate the length of the filename without the "-compressed" part
    int prefix_length = pos - argv[1];

    // Construct the output filename
    strncpy(out, argv[1], prefix_length);
    out[prefix_length] = '\0'; // Null-terminate the string
    strcat(out, "-decompressed.");

    char buff[1];
    // Read file extension length and the extension itself
    fread(buff, 1, 1, input);
    int fsize = buff[0] - '0';
    while (fsize--) {
        unsigned char ch;
        fread(&ch, 1, 1, input); // Read each character of the extension
        strncat(out, (char*)&ch, 1); // Append character to the output filename
    }

    FILE *output = fopen(out, "w");

    if (output == NULL) {
        fprintf(stderr, "Error creating output file\n");
        fclose(input);
        free(out);
        return 3;
    }

    // Create and initialize a dynamic array of kv_pair_t structs
    const int capacity = 256;  

    Hash_map decode_map[256];

    if (decode_map == NULL) {
        fprintf(stderr, "Error allocating memory for decode map.\n");
        fclose(input);
        fclose(output);
        free(out);
        return 3;
    }

    int flag = 0;
    char s[20];

    // Read and fill the hashmap to decode later
    while(fread(buff, 1, 1, input)){
        if(buff[0] != '\0'){
            flag = 0;
            strncat(s, &buff[0], 1);
        }
        else{
            num_of_unique_chars++;
            flag++;
            if(flag == 2)break;
            decode_map[num_of_unique_chars].value = s[0];
 
            strncpy(decode_map[num_of_unique_chars].key, s + 1, strlen(s));
            decode_map[num_of_unique_chars].key[strlen(s)] = '\0';
            s[0] = '\0';
        }
    }

    // Read and store the padding, read extra null chars 
    fread(buff, 1, 1, input);
    int padding = buff[0] - '0';
    fread(buff, 1, 1, input);


    fread(buff, 1, 1, input);  // null


    int count = 0;
    int decimal = 0;

    char *binary = (char *)malloc(1);

    while(fread(buff, 1, 1, input)){
        decimal = buff[0];
        strcat(binary, dectobin(decimal));
    }

    int start; 
    char *unpadded;

    if (padding == 0) {
        // If there's no padding, prepend '0' to the binary sequence
        unpadded = (char *)malloc(strlen(binary) + 2); // +2 for extra '0' and null terminator
        unpadded[0] = '0';
        strcpy(unpadded + 1, binary); // Copy binary after the prepended '0'
        start = 1;
    } else {
        // If there's padding, use the binary sequence as is
        unpadded = binary;
        start = padding;
    }


    for (int i = 1; unpadded[start] != '\0'; i++) {
        char key[20]; // Adjust the size as needed
        strncpy(key, &unpadded[start], i);
        key[i] = '\0'; // Null-terminate the key string
       

        char value = findKey(decode_map, num_of_unique_chars, key);
        if (value != '\0') {
            fwrite(&value, 1, 1, output);
            start += i;
            i = 0;

        }
    }
      
    printf("File decompressed successfully\n");
    // Free dynamically allocated memory
    fclose(output);
    return 0;
}