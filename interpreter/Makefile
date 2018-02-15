
CC := gcc
CFLAGS := -Wall -Wextra -Wpedantic -Werror -Wfatal-errors

default: all

all: build/um-interpreter.out

build/um-interpreter.out: build/main.o build/file_array.o build/ram_array.o
	$(CC) $(CFLAGS) -o $@ $^

build/main.o: main.c
	mkdir -p build
	$(CC) $(CFLAGS) -c -o $@ $<

build/file_array.o: file_array.c file_array.h
	mkdir -p build
	$(CC) $(CFLAGS) -c -o $@ $<

build/ram_array.o: ram_array.c ram_array.h
	mkdir -p build
	$(CC) $(CFLAGS) -c -o $@ $<

.PHONY: default all
