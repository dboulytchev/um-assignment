#ifndef _FILE_UTILS_H_
#define _FILE_UTILS_H_

long file_size(FILE* filp);

char* read_binary_file(const char* filename, long* fsize);

#endif  // _FILE_UTILS_H_
