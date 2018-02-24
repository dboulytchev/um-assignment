#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "ram_array.h"
#include "file_utils.h"

static ram_array_t* __allocate_ram_array_uninit(platter size) {
  // assert(size > 0);
  if (size == 0) size = 1;
  ram_array_t* res = malloc(sizeof(ram_array_t) + (sizeof(platter) * (size - 1)));
  res->size = size;
  return res;
}

platter* ram_array_begin(ram_array_t* array) {
  return &array->first;
}

ram_array_t* allocate_ram_array(platter size) {
  ram_array_t* res = __allocate_ram_array_uninit(size);
  memset(ram_array_begin(res), 0, size * sizeof(platter));
  return res;
}

void free_ram_array(ram_array_t* array) {
  free(array);
}

ram_array_t* duplicate_ram_array(ram_array_t* array) {
  ram_array_t* new = __allocate_ram_array_uninit(array->size);
  memcpy(ram_array_begin(new), ram_array_begin(array), array->size * sizeof(platter));
  return new;
}

ram_array_t* ram_array_from_file(const char* filename) {
  // this function relies on the word size equal to 4
  assert(sizeof(platter) == 4);

  long fsize = 0;
  char* buf = read_binary_file(filename, &fsize);
  if (fsize % sizeof(platter) != 0) {
    fprintf(stderr, "Invalid input file format: '%s'\n", filename);
    exit(5);
  }

  ram_array_t* res = __allocate_ram_array_uninit(fsize / sizeof(platter));

  platter* src = (platter*)buf;
  platter* src_end = src + res->size;
  platter* dst = ram_array_begin(res);
  for (; src != src_end; ++src, ++dst) {
    uint8_t* bytes = (uint8_t*)src;
    *dst = ((platter) bytes[0] << 24)
      + ((platter) bytes[1] << 16)
      + ((platter) bytes[2] << 8)
      + (platter) bytes[3];
  }

  return res;
}

platter ram_array_get(ram_array_t* array, platter idx) {
  assert(idx < array->size);
  return ram_array_begin(array)[idx];
}

void ram_array_set(ram_array_t* array, platter idx, platter value) {
  assert(idx < array->size);
  ram_array_begin(array)[idx] = value;
}
