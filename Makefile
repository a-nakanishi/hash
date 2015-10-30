CC = gcc
CFLAGS = -std=c99 
PROGRAM = simulator
SOURCES = BloomFilter.c HashTable.c Operation.c MurmurHash.c test.c
HEADERS = BloomFilter.h HashTable.h Operation.h MurmurHash.h Value.h
OBJECTS = $(SOURCES:%.c=%.o)
FILES = $(SOURCES) $(HEADERS)
#OBJECTS = test.o BloomFilter.o HashTable.o Operation.o MurmurHash.o

out : $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(PROGRAM)

#$(CC) $(CFLAGS) -o $(PROGRAM) $(OBJECTS)
#test.o: test.c
#	$(CC) $(CFLAGS) -c test.c	
#BloomFilter.o: BloomFilter.c
#	$(CC) $(CFLAGS) -c BloomFilter.c	
#HashTable.o: HashTable.c
#	$(CC) $(CFLAGS) -c HashTable.c	
#Operation.o: Operation.c
#	$(CC) $(CFLAGS) -c Operation.c	
#MurmurHash.o: MurmurHash.c
#	$(CC) $(CFLAGS) -c MurmurHash.c	

clean :
	$(RM) $(OBJECTS)
all:
	clean $(PROGRAM)
