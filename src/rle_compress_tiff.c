#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <tiffio.h> // Include LibTIFF header

// Structure to hold a pixel
typedef struct {
    unsigned char r, g, b; // Red, Green, Blue components
} Pixel;

// Simple structure to hold image metadata
typedef struct {
    int width, height; // Image dimensions
    Pixel *pixels;    // Array of pixels
} Image;

// Function to load image and create array of pixels
uint32_t* loadImage(const char* filename, int* width, int* height) {
    TIFF* tif = TIFFOpen(filename, "r");
    if (!tif) {
        fprintf(stderr, "Failed to open TIFF file\n");
        return NULL;
    }

    // Get image dimensions
    TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, width);
    TIFFGetField(tif, TIFFTAG_IMAGELENGTH, height);
    uint32_t npixels = *width * *height;

    // Allocate memory for pixel array
    uint32_t* pixels = (uint32_t*)malloc(npixels * sizeof(uint32_t));
    if (!pixels) {
        fprintf(stderr, "Failed to allocate memory for pixel array\n");
        TIFFClose(tif);
        return NULL;
    }

    // Read image data
    if (!TIFFReadRGBAImageOriented(tif, *width, *height, pixels, ORIENTATION_TOPRIGHT, 0)) {
        fprintf(stderr, "Failed to read image data\n");
        free(pixels);
        TIFFClose(tif);
        return NULL;
    }

    TIFFClose(tif);
    return pixels;
}

// Function to compress image using Run-Length Encoding (RLE)
void compressRLE(const char* filename, FILE *output) {
    int width, height;

    // Load image and create array of pixels
    uint32_t* pixels = loadImage(filename, &width, &height);
    if (!pixels) {
        fprintf(stderr, "Failed to load image\n");
        return;
    }

    printf("Image loaded successfully: %d x %d\n", width, height);

    // Create Image structure
    Image input;
    input.width = width;
    input.height = height;
    input.pixels = (Pixel *)malloc(width * height * sizeof(Pixel));

    // Convert RGBA to RGB format
    for (int i = 0; i < width * height; i++) {
        uint32_t pixelValue = pixels[i];
        input.pixels[i].r = (pixelValue) & 0xFF; // Red
        input.pixels[i].g = (pixelValue >> 8) & 0xFF; // Green
        input.pixels[i].b = (pixelValue >> 16) & 0xFF;  // Blue
    }

    free(pixels); // Free allocated memory for pixel array

    // Write the width and height to the compressed file
    fwrite(&(input.width), sizeof(int), 1, output);
    fwrite(&(input.height), sizeof(int), 1, output);

    // Compressing the image
    int count;
    Pixel current;
    for (int i = 0; i < input.width * input.height; i++) {
        current = input.pixels[i];
        count = 1;
        while (i + 1 < input.width * input.height && 
               input.pixels[i + 1].r == current.r &&
               input.pixels[i + 1].g == current.g &&
               input.pixels[i + 1].b == current.b &&
               count < 255) {
            count++;
            i++;
        }
        fwrite(&count, sizeof(unsigned char), 1, output);
        fwrite(&current, sizeof(Pixel), 1, output);
    }

    printf("Compression completed successfully\n");
    printf("Compressed image dimensions: %d x %d\n", input.width, input.height);

    // Free allocated memory
    free(input.pixels);
}

int main(int argc, char *argv[]) {
    // Load image using stb_image

    if (argc != 2) {
        fprintf(stderr, "Invalid cmd line arg. Usage: ./a.out <input file>\n");
        return 1;
    }

    const char *input_image_filename = argv[1];

    // Extract base filename without extension
    char *base_filename = strdup(input_image_filename);
    char *ext = strrchr(base_filename, '.');
    if (ext != NULL) {
        *ext = '\0'; // Remove the extension
    }

    // Create output filename based on the input image filename
    char output_filename[256]; // Adjust the size as needed
    snprintf(output_filename, sizeof(output_filename), "%s_compressed.bin", base_filename);

    FILE *compressedFile = fopen(output_filename, "wb");
    if (compressedFile != NULL) {
        printf("File opened successfully for writing\n");
        compressRLE(input_image_filename, compressedFile);
        fclose(compressedFile);
    } else {
        printf("Failed to open file for writing\n");
    }

    return 0;
}
