all: main


arrays_manager.o: arrays_manager.c
	gcc -std=c99 -c arrays_manager.c -o arrays_manager.o

main.o: main.c
	gcc -std=c99 -c main.c -o main.o

main: main.o arrays_manager.o
	gcc -std=c99 main.o arrays_manager.o -o main
