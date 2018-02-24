
CC := gcc
CFLAGS := -Wall -Wextra -Wpedantic -m32
# CFLAGS_DEBUG := -Wall -Wextra -Wpedantic -g3 -m32

default: release

release: CFLAGS += -DNDEBUG -O3 -flto
release: all

debug: CFLAGS += -DDEBUG -g3 -O0
debug: all

all: build/um-interpreter.out

build/um-interpreter.out: build/main.o build/ram_array.o build/um_state.o build/operators.o build/file_utils.o
	mkdir -p build
	$(CC) $(CFLAGS) -o $@ $^

build/main.o: .FORCE
	mkdir -p build
	$(CC) $(CFLAGS) -c -o $@ main.c

build/ram_array.o: .FORCE
	mkdir -p build
	$(CC) $(CFLAGS) -c -o $@ ram_array.c

build/um_state.o: .FORCE
	mkdir -p build
	$(CC) $(CFLAGS) -c -o $@ um_state.c

build/operators.o: .FORCE
	mkdir -p build
	$(CC) $(CFLAGS) -c -o $@ operators.c

build/file_utils.o: .FORCE
	mkdir -p build
	$(CC) $(CFLAGS) -c -o $@ file_utils.c

clean:
	rm -r build

.PHONY: default release debug clean .FORCE

.FORCE:
