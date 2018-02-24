#ifndef _UM_STATE_H_
#define _UM_STATE_H_

#include "ram_array.h"

typedef struct {
  ram_array_t* zero_array;
  platter execution_finger;
  platter regs[8];
} um_state_t;


#define UM_STATE_INIT(__zero_array) { \
    .zero_array = __zero_array, \
    .execution_finger = 0, \
    .regs = { 0 }, \
  }

ram_array_t* get_ram_array(um_state_t* state, platter id);

platter array_get(um_state_t* state, platter array_id, platter idx);

void array_set(um_state_t* state, platter array_id, platter idx, platter value);

#endif  // _UM_STATE_H_
