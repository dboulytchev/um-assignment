
#include <stdlib.h>
#include <stdio.h>

#include "ram_array.h"

static struct array_base_vtable ram_array_vtable = {
  .get = ram_array_get,
  .set = ram_array_set,
  .destroy = destroy_ram_array,
};

void check_idx(ram_array_t* self, platter idx) {
  if (idx > self->size) {
    fprintf(stderr, "Index %d is greater than the size of the array: %d", idx, self->size);
    exit(5);
  }
}

ram_array_t* create_ram_array(platter size, platter id) {
  ram_array_t* res = malloc(sizeof(ram_array_t));
  *res = (ram_array_t) {
    .base = {
      .vtable = &ram_array_vtable,
      .id = id,
      .lnode = LIST_HEAD_INITIALIZER(res->base.lnode),
    },
    .size = size,
    .data = calloc(size, sizeof(platter)),
  };
  return res;
}

platter ram_array_get(struct array_base* base, platter idx) {
  ram_array_t* self = (ram_array_t*) base;
  check_idx(self, idx);
  return self->data[idx];
}

void ram_array_set(struct array_base* base, platter idx, platter value) {
  ram_array_t* self = (ram_array_t*) base;
  if (idx > self->size) {
    fprintf(stderr, "Index %d is greater than the size of the array: %d", idx, self->size);
    exit(4);
  }
  self->data[idx] = value;
}

void destroy_ram_array(struct array_base* base) {
  ram_array_t* self = (ram_array_t*) base;
  free(self->data);
}
