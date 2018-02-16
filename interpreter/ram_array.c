
#include <stdlib.h>
#include <stdio.h>

#include "ram_array.h"

void check_idx(ram_array_t* self, platter idx) {
  if (idx > self->size) {
    fprintf(stderr, "Index %d is greater than the size of the array: %d", idx, self->size);
    exit(5);
  }
}

static platter ram_array_get(struct array_base* base, platter idx) {
  ram_array_t* self = (ram_array_t*) base;
  check_idx(self, idx);
  return self->data[idx];
}

static void ram_array_set(struct array_base* base, platter idx, platter value) {
  ram_array_t* self = (ram_array_t*) base;
  if (idx > self->size) {
    fprintf(stderr, "Index %d is greater than the size of the array: %d", idx, self->size);
    exit(4);
  }
  self->data[idx] = value;
}

ram_array_t* create_ram_array(platter size, platter id) {
  ram_array_t* res = malloc(sizeof(ram_array_t));
  *res = (ram_array_t) {
    .base = {
      .get = ram_array_get,
      .set = ram_array_set,
      .id = id,
      .lnode = LIST_HEAD_INITIALIZER(res->base.lnode),
    },
    .size = size,
    .data = calloc(size, sizeof(platter)),
  };
  return res;
}
