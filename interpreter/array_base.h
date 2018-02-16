
#ifndef _ARRAY_BASE_H_
#define _ARRAY_BASE_H_

#include "platter.h"
#include "list.h"

typedef struct array_base array_base_t;

struct array_base_vtable {
  platter (*get)(array_base_t* self, platter idx);
  void (*set)(array_base_t* self, platter idx, platter value);
  void (*destroy)(array_base_t* self);
  array_base_t* (*clone)(array_base_t* self, platter clone_id);
};

struct array_base {
  struct array_base_vtable* vtable;
  platter id;
  struct list_head lnode;
};

inline static platter array_get(array_base_t* array, platter idx) {
  return array->vtable->get(array, idx);
}

inline static void array_set(array_base_t* array, platter idx, platter value) {
  array->vtable->set(array, idx, value);
}

inline static void destroy_array(array_base_t* array) {
  array->vtable->destroy(array);
}

inline static array_base_t* clone_array(array_base_t* array, platter clone_id) {
  return array->vtable->clone(array, clone_id);
}

inline static void destroy_array_base(array_base_t* array) {
  list_del(&array->lnode);
}

#endif // _ARRAY_BASE_H_
