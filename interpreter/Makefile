
CC := gcc
CFLAGS := -Wall -Wextra -Wpedantic -Werror -Wfatal-errors

default: all

all: build/um-interpreter.out

build/um-interpreter.out: build/main.o build/file_array.o build/ram_array.o
	$(CC) $(CFLAGS) -o $@ $^

build/main.o: .FORCE
	mkdir -p build
	$(CC) $(CFLAGS) -c -o $@ main.c

build/file_array.o: .FORCE
	mkdir -p build
	$(CC) $(CFLAGS) -c -o $@ file_array.c

build/ram_array.o: .FORCE
	mkdir -p build
	$(CC) $(CFLAGS) -c -o $@ ram_array.c

.PHONY: default all .FORCE
.FORCE:
