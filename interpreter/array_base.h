
#ifndef _ARRAY_BASE_H_
#define _ARRAY_BASE_H_

#include "platter.h"
#include "list.h"

struct array_base {
  platter (*get)(struct array_base* self, platter idx);
  void (*set)(struct array_base* self, platter idx, platter value);
  platter id;
  struct list_head lnode;
};

#endif // _ARRAY_BASE_H_
