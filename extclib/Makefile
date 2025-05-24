CC=gcc
LN=ld
CFLAGS=-Wall -std=c99

HEADERS=net.h hashtab.h tree.h type.h
SOURCES=net.c hashtab.c tree.c type.c
OBJECTS=net.o hashtab.o tree.o type.o

.PHONY: default
default:build link

build: $(HEADERS) $(SOURCES)
	$(CC) $(CFLAGS) -c $(SOURCES)
link: $(OBJECTS)
	$(LN) -r $(OBJECTS) -o extclib.o
clean:
	rm -f *.o *.i *.s
