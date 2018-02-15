
#include <stdlib.h>
#include <stdio.h>

#include "file_array.h"

platter file_array_get(struct array_base* base, platter idx) {
  file_array_t* self = (file_array_t*) base;
  if (fseek(self->filp, idx, SEEK_SET) != 0) {
    fprintf(stderr, "Error accessing file");
    exit(6);
  }

  platter ret;
  if (fread(&ret, sizeof(ret), 1, self->filp) != sizeof(ret)) {
    fprintf(stderr, "Error reading from file");
    exit(7);
  }

  return ret;
}

file_array_t* create_file_array(char* filename) {
  file_array_t* res = malloc(sizeof(file_array_t));
  *res = (file_array_t) {
    .base = {
      .get = file_array_get,
    },
    .filp = fopen(filename, "r"),
  };
  init_list_head(&res->base.lnode);
  return res;
}
