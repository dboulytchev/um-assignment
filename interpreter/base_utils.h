
#ifndef _BASE_UTILS_H_
#define _BASE_UTILS_H_

#define container_of(ptr, type, member) \
  ((type*)((char*)(ptr) - offsetof(type, member)))

#endif // _BASE_UTILS_H_
