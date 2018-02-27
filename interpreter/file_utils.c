#include <stdio.h>
#include <stdlib.h>
#include "file_utils.h"

long file_size(FILE* filp) {
  long pos = ftell(filp);
  fseek(filp, 0, SEEK_END);
  long fsize = ftell(filp);
  fseek(filp, pos, SEEK_SET);
  return fsize;
}

char* read_binary_file(const char* filename, long* fsize) {
  FILE* filp = fopen(filename, "rb");
  *fsize = file_size(filp);

  char* buf = malloc(*fsize);
  if (fread(buf, *fsize, 1, filp) != 1) {
    perror("Error reading file");
    exit(3);
  }

  return buf;
}
