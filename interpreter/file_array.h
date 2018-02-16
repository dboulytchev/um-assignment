
#ifndef _FILE_ARRAY_H_
#define _FILE_ARRAY_H_

#include <stdio.h>

#include "array_base.h"

typedef struct file_array {
  struct array_base base;
  FILE* filp;
} file_array_t;

file_array_t* create_file_array(const char* file_path, platter id);

platter file_array_get(file_array_t* self, platter idx);

void file_array_set(file_array_t* self, platter idx, platter value);

void destroy_file_array(file_array_t* self);

#endif // _FILE_ARRAY_H_
