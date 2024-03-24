COM_VAR = tests.txt
DECOM_VAR = tests-compressed.bin
compress: compress.c $(COM_VAR)
	$ gcc -o out compress.c -lm
	$ ./out $(COM_VAR)
	$ rm out

decompress: decompress.c $(DECOM_VAR)
	$ gcc -o out decompress.c -lm
	$ ./out $(DECOM_VAR)
	$ rm out