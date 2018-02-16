
#ifndef _ARRAY_BASE_H_
#define _ARRAY_BASE_H_

#include "platter.h"
#include "list.h"

struct array_base;

struct array_base_vtable {
  platter (*get)(struct array_base* self, platter idx);
  void (*set)(struct array_base* self, platter idx, platter value);
  void (*destroy)(struct array_base* self);
};

struct array_base {
  struct array_base_vtable* vtable;
  platter id;
  struct list_head lnode;
};

inline static platter array_get(struct array_base* array, platter idx) {
  return array->vtable->get(array, idx);
}

inline static void array_set(struct array_base* array, platter idx, platter value) {
  array->vtable->set(array, idx, value);
}

#endif // _ARRAY_BASE_H_
