
#ifndef _BASE_UTILS_H_
#define _BASE_UTILS_H_

#define offsetof(type, member) \
  ((size_t)&((type*)NULL)->member)

#define container_of(ptr, type, member) \
  ((type*)((char*)(ptr) - offsetof(type, member)))

#endif // _BASE_UTILS_H_
