all: main

CCFLAGS= -std=c99 -m32 -Wall -Wextra -Wshadow -Wpedantic -ggdb3

arrays_manager.o: arrays_manager.c
	gcc $(CCFLAGS) -c arrays_manager.c -o arrays_manager.o

main.o: main.c
	gcc $(CCFLAGS) -c main.c -o main.o

main: main.o arrays_manager.o
	gcc $(CCFLAGS) main.o arrays_manager.o -o main

clean:
	rm -f *.o main
