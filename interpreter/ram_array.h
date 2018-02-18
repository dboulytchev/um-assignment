
#ifndef _RAM_ARRAY_H_
#define _RAM_ARRAY_H_

#include "array_base.h"

typedef struct ram_array {
  struct array_base base;
  platter size;
  platter* data;
} ram_array_t;

ram_array_t* create_ram_array(platter size, platter id);

platter ram_array_get(ram_array_t* self, platter idx);

void ram_array_set(ram_array_t* self, platter idx, platter value);

ram_array_t* ram_array_clone(ram_array_t* self, platter clone_id);

void destroy_ram_array(ram_array_t* self);

#endif // _RAM_ARRAY_H_
