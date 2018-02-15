
#ifndef _FILE_ARRAY_H_
#define _FILE_ARRAY_H_

#include <stdio.h>

#include "array_base.h"

typedef struct file_array {
  struct array_base base;
  FILE* filp;
} file_array_t;

platter file_array_get(struct array_base* base, platter idx);

file_array_t* create_file_array(char* file_path);

#endif // _FILE_ARRAY_H_
