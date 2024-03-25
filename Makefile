CVAR = tests.txt
DVAR = tests-compressed.bin
compress: compress.c $(CVAR)
	$ gcc -o out compress.c -lm
	$ ./out $(CVAR)
	$ rm out

decompress: decompress.c $(DVAR)
	$ gcc -o out decompress.c -lm
	$ ./out $(DVAR)
	$ rm out