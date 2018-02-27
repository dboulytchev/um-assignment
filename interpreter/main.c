#include <stdio.h>
#include <stdint.h>	
#include <stdlib.h>
#include <byteswap.h>
#include "arrays_manager.h"

uint32_t regs[8];
uint32_t pointer = 0;

void conditional_move(int a, int b, int c) {
	if (regs[c]) {
		regs[a] = regs[b];
	}
}

void array_index(int a, int b, int c) {
	regs[a] = get_element(regs[b], regs[c]);
}

void array_amedment(int a, int b, int c) {
	put_element(regs[a], regs[b], regs[c]);
}

void addiction(int a, int b, int c) {
	regs[a] = regs[b] + regs[c];
}


void multiplication(int a, int b, int c) {
	regs[a] = regs[b] * regs[c];
}

void division(int a, int b, int c) {
	regs[a] = regs[b] / regs[c];
}

void not_and(int a, int b, int c) {
	regs[a] = ~(regs[b] & regs[c]);
}

void halt(int a, int b, int c) {
	exit(0);
}

void allocation(int a, int b, int c) {
	regs[b] = allocate_array(regs[c]);
}

void abandonment(int a, int b, int c) {
	delete_array(regs[c]);
}

void output(int a, int b, int c) {
	printf("%c", (char)regs[c]);
}

void input(int a, int b, int c) {

	char symbol = getc(stdin);

	if (symbol == EOF) {
		regs[c] = ~((uint32_t)0);
	} else {
		regs[c] = (uint32_t)symbol;
	}
}

void load_program(int a, int b, int c) {
	load(regs[b]);
	pointer = regs[c];
}

void (*commands[13])(int, int, int) = {&conditional_move, &array_index, 
									   &array_amedment, &addiction, 
									   &multiplication, &division, 
									   &not_and, &halt, 
									   &allocation, &abandonment, 
									   &output, &input, &load_program};

void orthography(int a, uint32_t value) {
	regs[a] = value;
}


void load_from_file(const char *file_name) {
	FILE *source = fopen(file_name, "rb"); 
	fseek(source, 0, SEEK_END);
	long length = ftell(source);
	rewind(source);
	uint32_t *buffer = (uint32_t *)malloc(length + 4);
	length /= 4;
	fread(buffer + 1, length * 4, 1, source);

	buffer[0] = length;
	// convert to big endian
	for (int i = 1; i <= length; i++) {
		buffer[i] = __bswap_32(buffer[i]);
	}

	load((uint32_t)buffer);

	fclose(source);
	free(buffer);
}

int get_operator_number(uint32_t instruction) {
	return instruction >> (32 - 4);
}

int get_a_register(uint32_t instruction) {
	return (instruction >> 6) & 7;
}

int get_b_register(uint32_t instruction) {
	return (instruction >> 3) & 7;
}

int get_c_register(uint32_t instruction) {
	return instruction & 7;
}

int get_orthography_a_register(uint32_t instruction) {
	return (instruction >> (32 - 7)) & 7;
}

int get_orthography_value(uint32_t instruction) {
	return instruction & ((1 << (32 - 7)) - 1);
}

int main(int argc, char const *argv[]) {

	if (argc != 2) {
		printf("usage:\n    ./main ../codex.umz\n");
		return 0;
	}

	load_from_file(argv[1]);

	while (1) {
		uint32_t instruction = get_element(0, pointer);
		pointer++;
		int operator = get_operator_number(instruction);
		if (operator == 13) {
			int a = get_orthography_a_register(instruction);
			int value = get_orthography_value(instruction);
			orthography(a, value);
			// fprintf(stderr, "%.8x op=%d, a=%d, val=%x\n", instruction, operator, a, value);
		} else {
			int a = get_a_register(instruction);
			int b = get_b_register(instruction);
			int c = get_c_register(instruction);
			// fprintf(stderr, "%.8x op=%d, a=%d, b=%d, c=%d\n", instruction, operator, a, b, c);
			commands[operator](a, b, c);
		};
	}


	return 0;
}