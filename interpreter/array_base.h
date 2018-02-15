
#include "platter.h"

typedef struct array_base {
  platter (*get)(struct array_base* self, platter idx);
} array_base_t;
