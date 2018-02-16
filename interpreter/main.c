
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
  state->regs[reg_a(cmd)] = array_get(array, state->regs[reg_c(cmd)]);
}

void array_amendment(um_state_t* state, platter cmd) {
  struct array_base* array = get_array(state, state->regs[reg_a(cmd)]);
  array_set(array, state->regs[reg_b(cmd)], state->regs[reg_c(cmd)]);
}

void addition(um_state_t* state, platter cmd) {
  state->regs[reg_a(cmd)] = state->regs[reg_b(cmd)] + state->regs[reg_c(cmd)];
}

void multiplication(um_state_t* state, platter cmd) {
  state->regs[reg_a(cmd)] = state->regs[reg_b(cmd)] * state->regs[reg_c(cmd)];
}

void division(um_state_t* state, platter cmd) {
  state->regs[reg_a(cmd)] = state->regs[reg_b(cmd)] / state->regs[reg_c(cmd)];
}

void not_and(um_state_t* state, platter cmd) {
  state->regs[reg_a(cmd)] = ~(state->regs[reg_b(cmd)] & state->regs[reg_c(cmd)]);
}

void halt(um_state_t* state, platter cmd) {
  (void)state;
  (void)cmd;
  fprintf(stderr, "The machine is halted");
  exit(0);
}

void allocation(um_state_t* state, platter cmd) {
  ram_array_t* array = create_ram_array(state->regs[reg_c(cmd)], state->regs[reg_b(cmd)]);
  list_append(&array->base.lnode, &state->arrays);
}

void abandonment(um_state_t* state, platter cmd) {
  platter array_id = state->regs[reg_c(cmd)];
  if (array_id == 0) {
    fprintf(stderr, "Trying to destroy zero array");
    exit(123);
  }
  struct array_base* array = get_array(state, array_id);
  list_del(&array->lnode);
  destroy_array(array);
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
  list_append(&zero_array->base.lnode, &state->arrays);
}

int main() {
}
