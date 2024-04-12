#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define WINDOW_SIZE 4096   // Size of the sliding window
#define LOOKAHEAD_SIZE 16  // Size of the lookahead buffer

// Structure to represent a match in LZ77 compression
struct Match {
    int distance;  // Distance to the match in the sliding window
    int length;    // Length of the match
};

// Function to find the longest match starting from the current position
struct Match find_longest_match(unsigned char* input, int input_length, int current_pos, int window_start) {
    struct Match best_match = {0, 0};

    // Iterate over the sliding window to find potential matches
    for (int i = window_start; i < current_pos; i++) {
        int len = 0;
        // Check for a match between the lookahead buffer and the sliding window
        while (input[i + len] == input[current_pos + len] && len < LOOKAHEAD_SIZE && current_pos + len < input_length) {
            len++;
        }
        // Update the best match if the current match is longer
        if (len > best_match.length) {
            best_match.distance = current_pos - i;
            best_match.length = len;
        }
    }
    // printf("Best match: Distance: %d, Length: %d\n", best_match.distance, best_match.length);
    return best_match;
}

// Function to compress data using LZ77
void compress_lz77(unsigned char* input, int input_length, FILE* output) {
    int window_start = 0;
    int current_pos = 0;

    // Compress the input data
    while (current_pos < input_length) {
        struct Match match = find_longest_match(input, input_length, current_pos, window_start);

        // Write the match to the output file
        fputc(match.distance >> 4, output);  // High 4 bits of distance
        fputc(((match.distance & 0x0F) << 4) | match.length, output);  // Low 4 bits of distance and length

        // Advance the current position by the length of the match
        current_pos += match.length;

        // Update the sliding window if needed
        if (current_pos - window_start > WINDOW_SIZE)
            window_start = current_pos - WINDOW_SIZE;
        // printf("Current position: %d\n", current_pos);
    }
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Usage: %s [input_file] [output_file]\n", argv[0]);
        return 1;
    }

    // Open the input file
    FILE* input_file = fopen(argv[1], "rb");
    if (input_file == NULL) {
        printf("Error opening input file.\n");
        return 1;
    }

    // Open the output file
    FILE* output_file = fopen(argv[2], "wb");
    if (output_file == NULL) {
        printf("Error opening output file.\n");
        fclose(input_file);
        return 1;
    }

    // Read the input file into memory
    fseek(input_file, 0, SEEK_END);
    long input_length = ftell(input_file);
    fseek(input_file, 0, SEEK_SET);
    unsigned char* input_data = (unsigned char*)malloc(input_length);
    fread(input_data, 1, input_length, input_file);
    fclose(input_file);

    printf("%s\n", input_data);

    // Compress the input data using LZ77
    compress_lz77(input_data, input_length, output_file);

    // Close the output file
    fclose(output_file);

    // Free memory
    free(input_data);

    printf("Compression completed successfully.\n");
    return 0;
}
