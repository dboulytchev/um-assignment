#include "arrays_manager.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

uint32_t *instructions;

uint32_t *get_array(uint32_t array_index) {
	return array_index == 0 ? instructions : (uint32_t *) array_index;
}

uint32_t get_element(uint32_t array_index, uint32_t element_index) {
	uint32_t *array = get_array(array_index);
	return array[element_index + 1];
}

void put_element(uint32_t array_index, uint32_t element_index, uint32_t value) {
	uint32_t *array = get_array(array_index);
	array[element_index + 1] = value;
}

uint32_t allocate_array(uint32_t array_size) {
	uint32_t *array = calloc(array_size + 1, 4);
	array[0] = array_size;
	return (uint32_t)array;
}

void delete_array(uint32_t array_index) {
	free((uint32_t *)array_index);
}

void load(uint32_t array_index) {
	if (array_index == 0) {
		return;
	}

	free(instructions);
	int size = *((uint32_t*)array_index) + 1;

	instructions = (uint32_t *)calloc(size, 4);
	memcpy(instructions, (uint32_t *) array_index, size * 4);
}
