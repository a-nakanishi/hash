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

object : $(SOURCES) $(HEADERS)
	$(CC) -c $(CFLAGS) $(SOURCES)

simulater:$(OBJECTS)
	$(CC) -o $(PROGRAM) $(OBJECTS)

test.o: test.c Value.h
	$(CC) -c $(CFLAGS) test.c	

BloomFilter.o: BloomFilter.c Value.h
	$(CC) -c $(CFLAGS) BloomFilter.c	

HashTable.o: HashTable.c Value.h
	$(CC) -c $(CFLAGS) HashTable.c	

Operation.o: Operation.c Value.h
	$(CC) -c $(CFLAGS) Operation.c	

MurmurHash.o: MurmurHash.c Value.h
	$(CC) -c $(CFLAGS) MurmurHash.c	

clean :
	$(RM) $(OBJECTS)
