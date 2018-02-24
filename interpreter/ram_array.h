#ifndef _RAM_ARRAY_H_
#define _RAM_ARRAY_H_

#include "platter.h"

typedef struct {
  platter size;
  platter first;
} ram_array_t;


ram_array_t* allocate_ram_array(platter size);

void free_ram_array(ram_array_t* array);

ram_array_t* duplicate_ram_array(ram_array_t* array);

ram_array_t* ram_array_from_file(const char* filename);

platter* ram_array_begin(ram_array_t* array);

platter ram_array_get(ram_array_t* array, platter idx);

void ram_array_set(ram_array_t* array, platter idx, platter value);

#endif  // _RAM_ARRAY_H_
