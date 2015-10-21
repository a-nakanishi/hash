#Makefile
test: test.c
	gcc -std=c99 test.c
	./a.out
