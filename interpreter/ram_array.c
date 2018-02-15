
#include <stdlib.h>
#include <stdio.h>

#include "ram_array.h"

platter ram_array_get(struct array_base* base, platter idx) {
  ram_array_t* self = (ram_array_t*) base;
  if (idx > self->size) {
    fprintf(stderr, "Index %d is greater than the size of the array: %d", idx, self->size);
    exit(5);
  }
  return self->data[idx];
}

ram_array_t* create_ram_array(platter size) {
  ram_array_t* res = malloc(sizeof(ram_array_t));
  *res = (ram_array_t) {
    .base = {
      .get = ram_array_get,
    },
    .size = size,
    .data = calloc(size, sizeof(platter)),
  };
  init_list_head(&res->base.lnode);
  return res;
}
