
#include <stdlib.h>

#include "platter.h"
#include "file_array.h"
#include "ram_array.h"


struct um_state;
typedef void (*operator)(struct um_state* state, platter cmd);

typedef struct um_state {
  struct list_head arrays;
  platter regs[8];
  operator ops[14];
} um_state_t;

struct array_base* get_array(um_state_t* state, platter id) {
  struct list_head* cur;
  list_for_each(cur, &state->arrays) {
    struct array_base* array = list_entry(cur, struct array_base, lnode);
    if (array->id == id) {
      return array;
    }
  }
  fprintf(stderr, "Error with id %d doesn't exist", id);
  exit(8);
}

size_t reg_a(platter cmd) {
  return (cmd >> 6) & 7;
}

size_t reg_b(platter cmd) {
  return (cmd >> 3) & 7;
}

size_t reg_c(platter cmd) {
  return cmd & 7;
}

void conditional_move(um_state_t* state, platter cmd) {
  if (state->regs[reg_c(cmd)]) {
    state->regs[reg_a(cmd)] = state->regs[reg_b(cmd)];
  }
}

void array_index(um_state_t* state, platter cmd) {
  struct array_base* array = get_array(state, state->regs[reg_b(cmd)]);
  state->regs[reg_a(cmd)] = array->get(array, state->regs[reg_c(cmd)]);
}

void array_amendment(um_state_t* state, platter cmd) {
  struct array_base* array = get_array(state, state->regs[reg_a(cmd)]);
  array->set(array, state->regs[reg_b(cmd)], state->regs[reg_c(cmd)]);
}

void init_um_state(um_state_t* state, const char* filename) {
  *state = (um_state_t) {
    .arrays = LIST_HEAD_INITIALIZER(state->arrays),
    .regs = { 0 },
    .ops = {
      conditional_move,
    },
  };
  file_array_t* zero_array = create_file_array(filename, 0);
  list_add(&zero_array->base.lnode, &state->arrays);
}

int main() {
}
