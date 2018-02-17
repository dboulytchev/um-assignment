#ifndef ARRAYS_MANAGER_H
#define ARRAYS_MANAGER_H

#include <stdint.h>

uint32_t get_element(uint32_t array_index, uint32_t element_index);

void put_element(uint32_t array_index, uint32_t element_index, uint32_t value);

void allocate_array(uint32_t array_index, uint32_t array_size);

void delete_array(uint32_t array_index);

void load_from_array(uint32_t array_index);

void load(uint32_t *array, int array_size);

#endif 