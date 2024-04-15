#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <zlib.h>

#define CHUNK_SIZE 16384

int compress_folder(const char *source_folder, const char *dest_file);
int decompress_folder(const char *source_file, const char *dest_folder);

int main(int argc, char *argv[]) {
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
            printf("Compression failed with error code %d\n", ret);
            return 1;
        }
        printf("Compression successful\n");
    } else if (strcmp(command, "decompress") == 0) {
        int ret = decompress_folder(source, dest);
        if (ret != Z_OK) {
            printf("Decompression failed with error code %d\n", ret);
            return 1;
        }
        printf("Decompression successful\n");
    } else {
        printf("Invalid command. Please use 'compress' or 'decompress'\n");
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
        perror("Error opening destination file");
        return Z_ERRNO;
    }

    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;

    ret = deflateInit(&strm, Z_DEFAULT_COMPRESSION);
    if (ret != Z_OK) {
        printf("Compression initialization failed with error code %d\n", ret);
        fclose(dest);
        return ret;
    }

    dir = opendir(source_folder);
    if (!dir) {
        perror("Error opening source folder");
        deflateEnd(&strm);
        fclose(dest);
        return Z_ERRNO;
    }

    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_REG) {
            char filepath[PATH_MAX];
            snprintf(filepath, sizeof(filepath), "%s/%s", source_folder, entry->d_name);

            FILE *source_file = fopen(filepath, "rb");
            if (!source_file) {
                perror("Error opening source file");
                deflateEnd(&strm);
                fclose(dest);
                closedir(dir);
                return Z_ERRNO;
            }

            printf("Compressing file: %s\n", entry->d_name);

            do {
                strm.avail_in = fread(in, 1, CHUNK_SIZE, source_file);
                if (strm.avail_in == 0) {
                    if (feof(source_file)) {
                        break;
                    } else if (ferror(source_file)) {
                        printf("Error reading from source file\n");
                        deflateEnd(&strm);
                        fclose(source_file);
                        fclose(dest);
                        closedir(dir);
                        return Z_ERRNO;
                    }
                }
                strm.next_in = in;

                do {
                    strm.avail_out = CHUNK_SIZE;
                    strm.next_out = out;
                    ret = deflate(&strm, Z_FINISH);
                    if (ret == Z_STREAM_ERROR) {
                        printf("Compression error\n");
                        deflateEnd(&strm);
                        fclose(source_file);
                        fclose(dest);
                        closedir(dir);
                        return ret;
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
        perror("Error opening source file");
        return Z_ERRNO;
    }

    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;

    ret = inflateInit(&strm);
    if (ret != Z_OK) {
        printf("Decompression initialization failed with error code %d\n", ret);
        fclose(source);
        return ret;
    }

    mkdir(dest_folder, 0777); // Create the destination folder if it doesn't exist

    while (1) {
        char filename[PATH_MAX];
        snprintf(filename, sizeof(filename), "%s/file_%d.txt", dest_folder, file_count++); // Generate a unique file name
        FILE *dest_file = fopen(filename, "wb");
        if (!dest_file) {
            printf("Error creating destination file\n");
            inflateEnd(&strm);
            fclose(source);
            return Z_ERRNO;
        }

        while (1) {
            strm.avail_in = fread(in, 1, CHUNK_SIZE, source);
            if (ferror(source)) {
                printf("Error reading from source file\n");
                inflateEnd(&strm);
                fclose(source);
                fclose(dest_file);
                return Z_ERRNO;
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
                        printf("Decompression error\n");
                        inflateEnd(&strm);
                        fclose(source);
                        fclose(dest_file);
                        return ret;
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
