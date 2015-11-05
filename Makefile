#Makefile
#	gcc -c -std=c99 test.c BloomFilter.c HashTable.c MurmurHash.c Operation.c
#	gcc -o out test.o BloomFilter.o HashTable.o MurmurHash.o Operation.o 
#	./out
CC = gcc
CFLAGS = -std=c99 
PROGRAM = simulater
SOURCES = test.c  BloomFilter.c HashTable.c Operation.c MurmurHash.c
HEADERS = Value.h BloomFilter.h HashTable.h Operation.h MurmurHash.h
OBJECTS = test.o  BloomFilter.o HashTable.o Operation.o MurmurHash.o

all : $(SOURCES)
	$(CC) -c $(CFLAGS) $(SOURCES)
	$(CC) -o $(PROGRAM) $(OBJECTS)
	./$(PROGRAM)

object : $(SOURCES)
	$(CC) -c $(CFLAGS) $(SOURCES)

simulater:$(OBJECTS)
	$(CC) -o $(PROGRAM) $(OBJECTS)

clean :
	$(RM) $(OBJECTS)
