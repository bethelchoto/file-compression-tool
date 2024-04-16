#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <zlib.h>
#include "error_handling.h"

#define CHUNK_SIZE 16384

int compress_folder(const char *source_folder, const char *dest_file);
int decompress_folder(const char *source_file, const char *dest_folder);

int main(int argc, char *argv[]) {

    setup_signal_handlers();

    if (argc != 4) {
        printf("Usage: %s <command> <source> <destination>\n", argv[0]);
        return 1;
    }

    char *command = argv[1];
    char *source = argv[2];
    char *dest = argv[3];

    if (strcmp(command, "compress") == 0) {
        int ret = compress_folder(source, dest);
        if (ret != Z_OK) {
            report_error(ERROR_UNEXPECTED_EOF);
            return 7;
        }
        printf("Compression successful\n");
    } else if (strcmp(command, "decompress") == 0) {
        int ret = decompress_folder(source, dest);
        if (ret != Z_OK) {
            report_error(ERROR_UNEXPECTED_EOF);
            return 7;
        }
        printf("Decompression successful\n");
    } else {
        report_error(ERROR_INVALID_CMD_ARG);
        return 1;
    }

    return 0;
}

int compress_folder(const char *source_folder, const char *dest_file) {
    DIR *dir;
    struct dirent *entry;
    int ret;
    z_stream strm;
    unsigned char in[CHUNK_SIZE];
    unsigned char out[CHUNK_SIZE];

    FILE *dest = fopen(dest_file, "wb");
    if (!dest) {
        report_error(ERROR_CREATE_OUTPUT_FILE);
        return 6;
    }

    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;

    ret = deflateInit(&strm, Z_DEFAULT_COMPRESSION);
    if (ret != Z_OK) {
        report_error(ERROR_UNEXPECTED_EOF);
        fclose(dest);
        return 7;
    }

    dir = opendir(source_folder);
    if (!dir) {
        report_error(ERROR_INVALID_FILENAME);
        deflateEnd(&strm);
        fclose(dest);
        return 4;
    }

    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_REG) {
            char filepath[PATH_MAX];
            snprintf(filepath, sizeof(filepath), "%s/%s", source_folder, entry->d_name);

            FILE *source_file = fopen(filepath, "rb");
            if (!source_file) {
                report_error(ERROR_OPEN_INPUT_FILE);
                deflateEnd(&strm);
                fclose(dest);
                closedir(dir);
                return 2;
            }

            printf("Compressing file: %s\n", entry->d_name);

            do {
                strm.avail_in = fread(in, 1, CHUNK_SIZE, source_file);
                if (strm.avail_in == 0) {
                    if (feof(source_file)) {
                        break;
                    } else if (ferror(source_file)) {
                        report_error(ERROR_READ_FILE);
                        deflateEnd(&strm);
                        fclose(source_file);
                        fclose(dest);
                        closedir(dir);
                        return 5;
                    }
                }
                strm.next_in = in;

                do {
                    strm.avail_out = CHUNK_SIZE;
                    strm.next_out = out;
                    ret = deflate(&strm, Z_FINISH);
                    if (ret == Z_STREAM_ERROR) {
                        report_error(ERROR_UNEXPECTED_EOF)
                        deflateEnd(&strm);
                        fclose(source_file);
                        fclose(dest);
                        closedir(dir);
                        return 7;
                    }
                    fwrite(out, 1, CHUNK_SIZE - strm.avail_out, dest);
                } while (strm.avail_out == 0);
            } while (ret != Z_STREAM_END);

            fclose(source_file);
        }
    }

    closedir(dir);
    deflateEnd(&strm);
    fclose(dest);

    return Z_OK;
}


int decompress_folder(const char *source_file, const char *dest_folder) {
    int ret;
    z_stream strm;
    unsigned char in[CHUNK_SIZE];
    unsigned char out[CHUNK_SIZE];
    int file_count = 0;

    FILE *source = fopen(source_file, "rb");
    if (!source) {
        report_error(ERROR_OPEN_INPUT_FILE);
        return 2;
    }

    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;

    ret = inflateInit(&strm);
    if (ret != Z_OK) {
        report_error(ERROR_UNEXPECTED_EOF);
        fclose(source);
        return 7;
    }

    mkdir(dest_folder, 0777); // Create the destination folder if it doesn't exist

    while (1) {
        char filename[PATH_MAX];
        snprintf(filename, sizeof(filename), "%s/file_%d.txt", dest_folder, file_count++); // Generate a unique file name
        FILE *dest_file = fopen(filename, "wb");
        if (!dest_file) {
            report_error(ERROR_CREATE_OUTPUT_FILE);
            inflateEnd(&strm);
            fclose(source);
            return 6;
        }

        while (1) {
            strm.avail_in = fread(in, 1, CHUNK_SIZE, source);
            if (ferror(source)) {
                report_error(ERROR_INVALID_FILENAME);
                inflateEnd(&strm);
                fclose(source);
                fclose(dest_file);
                return 4;
            }
            if (strm.avail_in == 0)
                break;
            strm.next_in = in;

            do {
                strm.avail_out = CHUNK_SIZE;
                strm.next_out = out;
                ret = inflate(&strm, Z_NO_FLUSH);
                switch (ret) {
                    case Z_NEED_DICT:
                    case Z_DATA_ERROR:
                    case Z_MEM_ERROR:
                        // printf("Decompression error\n");
                        report_error(ERROR_UNEXPECTED_EOF);
                        inflateEnd(&strm);
                        fclose(source);
                        fclose(dest_file);
                        return 7;
                }
                fwrite(out, 1, CHUNK_SIZE - strm.avail_out, dest_file);
            } while (strm.avail_out == 0);
        }

        fclose(dest_file);

        if (ret == Z_STREAM_END)
            break;
    }

    inflateEnd(&strm);
    fclose(source);

    return Z_OK;
}
