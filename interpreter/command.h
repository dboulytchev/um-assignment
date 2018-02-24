#ifndef _COMMAND_H_
#define _COMMAND_H_

#include "platter.h"

typedef union {
  struct { platter c: 3; platter b: 3; platter a: 3; platter _skip1: 19; platter op: 4; };
  struct { platter value: 25; platter spec: 3; platter _skip2: 4; };
  platter raw;
} command_t;

#endif  // _COMMAND_H_
