#include "um_state.h"

ram_array_t* get_ram_array(um_state_t* state, platter id) {
  return id == 0 ? state->zero_array : (ram_array_t*) id;
}

platter array_get(um_state_t* state, platter array_id, platter idx) {
  return ram_array_get(get_ram_array(state, array_id), idx);
}

void array_set(um_state_t* state, platter array_id, platter idx, platter value) {
  ram_array_set(get_ram_array(state, array_id), idx, value);
}
