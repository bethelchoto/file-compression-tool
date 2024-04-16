all: compressimg compresstiff compressdoc compresszip decompressimg decompressdoc endmsg
compressimg: src/rle_compress_stbi.c src/stb_image_write.h src/stb_image.h src/error_handling.c src/error_handling.h
	@gcc -o executables/img_compress src/rle_compress_stbi.c src/error_handling.c  -lm

compresstiff: src/rle_compress_tiff.c src/error_handling.c src/error_handling.h
	@gcc -o executables/tiff_compress src/rle_compress_tiff.c src/error_handling.c  -lm -ltiff

compressdoc: src/compress.c src/error_handling.c src/error_handling.h
	@gcc -o executables/doc_compress src/compress.c src/error_handling.c 

compresszip: src/file_zipper.c src/error_handling.c src/error_handling.h
	@gcc -o executables/zipper src/file_zipper.c src/error_handling.c -lz

decompressimg: src/rle_decompress.c src/stb_image_write.h src/stb_image.h src/error_handling.c src/error_handling.h
	@gcc -o executables/img_decompress src/rle_decompress.c src/error_handling.c  -lm

decompressdoc: src/decompress.c src/error_handling.c src/error_handling.h
	@gcc -o executables/doc_decompress src/decompress.c src/error_handling.c  -lm

endmsg:
	@echo "Compilation Successful"