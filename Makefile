CFLAGS=-O3 -Wall
C=clang -stdlib=libc -std=c11
LDFLAGS =
LDLIBS = 
BINDIR = ./bin
BINNAME := simulator
TARGET	:= $(BINDIR)/$(BINNAME)
SUBDIRS =
SOURCES := $(wildcard *.c) $(foreach d, $(SUBDIRS), $(wildcard $(d)/*.c))
HEADERS := $(wildcard *.h) $(foreach d, $(SUBDIRS), $(wildcard $(d)/*.h))
OBJECTS := $(SOURCES:%.c=%.o)
FILES     := $(HEADERS) $(SOURCES) $(MAKEFILE_LIST)
CXXFLAGS += $(foreach d, $(SUBDIRS), -I./$(d))

ifeq ($(strip $(BINDIR)), "")
  BINDIR = ./
endif

all: $(TARGET)
$(TARGET): $(OBJECTS)
	@[ -d $(BINDIR) ] || mkdir -p $(BINDIR)
	$(LINK.cc) $^ $(LOADLIBES) $(LDLIBS) -o $@
clean:
	@$(RM) $(OBJECTS) $(TARGET) *.bak *~ core* GTAGS GSYMS GRTAGS GPATH
depend:
	makedepend -Y -f $(MAKEFILE_LIST) -- $(foreach d, $(SUBDIRS), -I./$(d)) -- $(SOURCES)
.PHONY: clean depend
# DO NOT DELETE

hash.o: HashTable.h entry.h
main.o: BloomFilter.h HashTable.h hash.h entry.h

#Makefile
#test: test.c
#	gcc -std=c99 test.c
#	./a.out
