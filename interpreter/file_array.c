
#include <stdlib.h>
#include <stdio.h>

#include "file_array.h"

static void fseek_or_fail(file_array_t* self, platter position) {
  if (fseek(self->filp, position, SEEK_SET) != 0) {
    fprintf(stderr, "Error accessing file");
    exit(6);
  }
}

static platter file_array_get(struct array_base* base, platter idx) {
  file_array_t* self = (file_array_t*) base;
  fseek_or_fail(self, idx);

  platter ret;
  if (fread(&ret, sizeof(ret), 1, self->filp) != sizeof(ret)) {
    fprintf(stderr, "Error reading from file");
    exit(7);
  }

  return ret;
}

static void file_array_set(struct array_base* base, platter idx, platter value) {
  file_array_t* self = (file_array_t*) base;
  fseek_or_fail(self, idx);

  if (fwrite(&value, sizeof(value), 1, self->filp) != sizeof(value)) {
    fprintf(stderr, "Error writing to file");
    exit(9);
  }
}

file_array_t* create_file_array(const char* filename, platter id) {
  file_array_t* res = malloc(sizeof(file_array_t));
  *res = (file_array_t) {
    .base = {
      .get = file_array_get,
      .set = file_array_set,
      .id = id,
      .lnode = LIST_HEAD_INITIALIZER(res->base.lnode),
    },
    .filp = fopen(filename, "rw"),
  };
  return res;
}
