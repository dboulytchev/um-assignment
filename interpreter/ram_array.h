
#include "array_base.h"

typedef struct ram_array {
  struct array_base base;
  platter size;
  platter* data;
} ram_array_t;

platter ram_array_get(array_base_t* base, platter idx);

ram_array_t* create_ram_array(platter size);
