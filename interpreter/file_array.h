
#include <stdio.h>

#include "array_base.h"

typedef struct file_array {
  struct array_base base;
  FILE* filp;
} file_array_t;

platter file_array_get(array_base_t* base, platter idx);

file_array_t* create_file_array(char* file_path);
