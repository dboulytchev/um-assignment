
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "ram_array.h"

static struct array_base_vtable ram_array_vtable = {
  .get = (typeof(ram_array_vtable.get)) ram_array_get,
  .set = (typeof(ram_array_vtable.set)) ram_array_set,
  .destroy = (typeof(ram_array_vtable.destroy)) destroy_ram_array,
  .clone = (typeof(ram_array_vtable.clone)) ram_array_clone,
};

void check_idx(ram_array_t* self, platter idx) {
  if (idx > self->size) {
    fprintf(stderr, "Index %d is greater than the size of the array: %d\n", idx, self->size);
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

platter ram_array_get(ram_array_t* self, platter idx) {
  check_idx(self, idx);
  return self->data[idx];
}

void ram_array_set(ram_array_t* self, platter idx, platter value) {
  if (idx > self->size) {
    fprintf(stderr, "Index %d is greater than the size of the array: %d\n", idx, self->size);
    exit(4);
  }
  self->data[idx] = value;
}

ram_array_t* ram_array_clone(ram_array_t* self, platter clone_id) {
  ram_array_t* res = create_ram_array(self->size, clone_id);
  memcpy(res->data, self->data, sizeof(*self->data) * self->size);
  return res;
}

void destroy_ram_array(ram_array_t* self) {
  free(self->data);
  destroy_array_base((array_base_t*) self);
}
