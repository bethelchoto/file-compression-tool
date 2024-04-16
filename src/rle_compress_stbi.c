#include <stdio.h>
#include <stdlib.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h" // Include stb_image header
#include "error_handling.h"

// Structure to hold a pixel
typedef struct {
    unsigned char r, g, b; // Red, Green, Blue components
} Pixel;

// Simple structure to hold image metadata
typedef struct {
    int width, height; // Image dimensions
    Pixel *pixels;    // Array of pixels
} Image;

// Function to compress image using Run-Length Encoding (RLE)
void compressRLE(Image *input, FILE *output) {
    // Write the width and height to the compressed file
    fwrite(&(input->width), sizeof(int), 1, output);
    fwrite(&(input->height), sizeof(int), 1, output);

    int count;
    Pixel current;
    
    for (int i = 0; i < input->width * input->height; i++) {
        current = input->pixels[i];
        count = 1;
        while (i + 1 < input->width * input->height && 
               input->pixels[i + 1].r == current.r &&
               input->pixels[i + 1].g == current.g &&
               input->pixels[i + 1].b == current.b &&
               count < 255) {
            count++;
            i++;
        }
        fwrite(&count, sizeof(unsigned char), 1, output);
        fwrite(&current, sizeof(Pixel), 1, output);
    }
}

int main(int argc, char *argv[]) {
    // Load image using stb_image

    setup_signal_handlers();

    if (argc != 2) {
        report_error(ERROR_INVALID_CMD_ARG);
        return 1;
    }

    const char *input_image_filename = argv[1];

    int width, height, channels;
    unsigned char *image_data = stbi_load(input_image_filename, &width, &height, &channels, STBI_rgb);

    if (image_data == NULL) {
        report_error(ERROR_READ_FILE);
        return 5;
    }

    printf("Image loaded successfully\n");

    // Create Image structure
    Image input;
    input.width = width;
    input.height = height;
    input.pixels = (Pixel *)malloc(width * height * sizeof(Pixel));

    // Convert image data to Pixel array
    for (int i = 0; i < width * height; i++) {
        input.pixels[i].r = image_data[i * 3];
        input.pixels[i].g = image_data[i * 3 + 1];
        input.pixels[i].b = image_data[i * 3 + 2];
    }

    printf("Image converted to pixel array successfully\n"); 

    // Extract base filename without extension
    char *base_filename = strdup(input_image_filename);
    char *ext = strrchr(base_filename, '.');
    if (ext != NULL) {
        *ext = '\0'; // Remove the extension
    }

    // Create output filename based on the input image filename
    char output_filename[256]; // Adjust the size as needed
    snprintf(output_filename, sizeof(output_filename), "%s_compressed.bin", base_filename);
    
    // Compressing the image
    FILE *compressedFile = fopen(output_filename, "wb");
    if (compressedFile != NULL) {
        printf("File opened successfully for writing\n");
        compressRLE(&input, compressedFile);
        fclose(compressedFile);
        printf("Compression completed successfully\n");
        printf("Compressed image dimensions: %d x %d\n", input.width, input.height);
    } else {
        report_error(ERROR_OPEN_INPUT_FILE);
        return 2;
    }


    return 0;
}
