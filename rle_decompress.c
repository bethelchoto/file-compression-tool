#include <stdio.h>
#include <stdlib.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h" // Include stb_image header
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h" // Include stb_image_write header

// Structure to hold a pixel
typedef struct {
    unsigned char r, g, b; // Red, Green, Blue components
} Pixel;

// Simple structure to hold image metadata
typedef struct {
    int width, height; // Image dimensions
    Pixel *pixels;    // Array of pixels
} Image;

// Function to decompress image using Run-Length Encoding (RLE)
Image* decompressRLE(FILE *input) {
    Image *output = malloc(sizeof(Image));

    // Read the width and height from the compressed file
    fread(&(output->width), sizeof(int), 1, input);
    fread(&(output->height), sizeof(int), 1, input);

    output->pixels = malloc(output->width * output->height * sizeof(Pixel));

    int count;
    Pixel current;
    int index = 0;

    while (fread(&count, sizeof(unsigned char), 1, input)) {
        fread(&current, sizeof(Pixel), 1, input);
        for (int i = 0; i < count; i++) {
            output->pixels[index++] = current;
        }
    }

    return output;
}

// Function to save image using stb_image_write
void saveImage(char* filename, int width, int height, Pixel* pixels) {
    stbi_write_jpg(filename, width, height, 3, pixels, width * sizeof(Pixel));
}

int main(int argc, char **argv) {

    if (argc != 2)
    {
        printf("this is not how we decompress here- ./out <file>\n");
        exit(1);
    }
    
    const char* filename = argv[1];

    char *base_filename = strdup(filename);
    char *ext = strstr(base_filename, "-compressed.");
    if (ext != NULL) {
        *ext = '\0'; // Remove the extension
    }

    // Decompressing the image
    FILE *decompressedFile = fopen(filename, "rb");
    if (decompressedFile != NULL) {
        printf("File opened successfully for reading\n");
        Image *output = decompressRLE(decompressedFile);
        fclose(decompressedFile);
        printf("Decompression completed successfully\n");

        printf("Decompressed image dimensions: %d x %d\n", output->width, output->height);

        // Create output filename based on the input image filename
        char output_filename[256]; // Adjust the size as needed
        snprintf(output_filename, sizeof(output_filename), "%s-decompressed.jpg", base_filename);
        saveImage(output_filename, output->width, output->height, output->pixels);
        printf("Decompressed image saved as %s\n", output_filename);

        free(output->pixels);
        free(output);
    } else {
        printf("Failed to open file for reading\n");
    }


    return 0;
}
