
#ifndef _RAM_ARRAY_H_
#define _RAM_ARRAY_H_

#include "array_base.h"

typedef struct ram_array {
  struct array_base base;
  platter size;
  platter* data;
} ram_array_t;

ram_array_t* create_ram_array(platter size, platter id);

platter ram_array_get(struct array_base* base, platter idx);

void ram_array_set(struct array_base* base, platter idx, platter value);

void destroy_ram_array(struct array_base* base);

#endif // _RAM_ARRAY_H_
