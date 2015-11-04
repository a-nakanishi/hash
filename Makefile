#Makefile
test: test.c
	gcc -o -std=c99 test test.c -lm
	./a.out
