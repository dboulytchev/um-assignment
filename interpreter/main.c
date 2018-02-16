
#include <stdlib.h>

#include "platter.h"
#include "file_array.h"
#include "ram_array.h"


struct um_state;
typedef void (*operator)(struct um_state* state, platter cmd);

typedef struct um_state {
  struct list_head arrays;
  platter regs[8];
  operator operators[14];
  uint64_t execution_finger;
} um_state_t;

void destroy_um_state(um_state_t* state) {
  struct list_head* cur;
  struct list_head* tmp;
  list_for_each_safe(cur, tmp, &state->arrays) {
    destroy_array(list_entry(cur, array_base_t, lnode));
  }
  fflush(stdout);
}

array_base_t* get_array(um_state_t* state, platter id) {
  struct list_head* cur;
  list_for_each(cur, &state->arrays) {
    array_base_t* array = list_entry(cur, array_base_t, lnode);
    if (array->id == id) {
      return array;
    }
  }
  fprintf(stderr, "Array with id '%d' doesn't exist\n", id);
  exit(8);
}

static inline size_t reg_a(platter cmd) {
  return (cmd >> 6) & 7;
}

static inline size_t reg_b(platter cmd) {
  return (cmd >> 3) & 7;
}

static inline size_t reg_c(platter cmd) {
  return cmd & 7;
}

void conditional_move(um_state_t* state, platter cmd) {
  if (state->regs[reg_c(cmd)]) {
    state->regs[reg_a(cmd)] = state->regs[reg_b(cmd)];
  }
}

void array_index(um_state_t* state, platter cmd) {
  array_base_t* array = get_array(state, state->regs[reg_b(cmd)]);
  state->regs[reg_a(cmd)] = array_get(array, state->regs[reg_c(cmd)]);
}

void array_amendment(um_state_t* state, platter cmd) {
  array_base_t* array = get_array(state, state->regs[reg_a(cmd)]);
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
  destroy_um_state(state);
  exit(0);
}

void allocation(um_state_t* state, platter cmd) {
  ram_array_t* array = create_ram_array(state->regs[reg_c(cmd)], state->regs[reg_b(cmd)]);
  list_append(&array->base.lnode, &state->arrays);
}

void abandonment(um_state_t* state, platter cmd) {
  platter array_id = state->regs[reg_c(cmd)];
  if (array_id == 0) {
    fprintf(stderr, "Trying to destroy zero array\n");
    exit(123);
  }
  array_base_t* array = get_array(state, array_id);
  destroy_array(array);
}

void output(um_state_t* state, platter cmd) {
  platter value = state->regs[reg_c(cmd)];
  if (value > 255) {
    fprintf(stderr, "Trying to output too large number: %d\n", value);
    exit(77);
  }
  printf("%c", (char)value);
}

void input(um_state_t* state, platter cmd) {
  char ch;
  state->regs[reg_c(cmd)] = (scanf("%c", &ch) == EOF) ? -1 : ch;
}

void load_program(um_state_t* state, platter cmd) {
  platter array_id = state->regs[reg_b(cmd)];
  if (array_id == 0) {
    return;
  }
  destroy_array(get_array(state, 0));
  array_base_t* array = get_array(state, array_id);
  array_base_t* clone = clone_array(array, 0);
  list_prepend(&clone->lnode, &state->arrays);
}

static inline size_t reg_special(platter cmd) {
  return (cmd >> 25) & 7;
}

static inline platter value_special(platter cmd) {
  return cmd & ((1 << 25) - 1);
}

void load_value(um_state_t* state, platter cmd) {
  state->regs[reg_special(cmd)] = value_special(cmd);
}

void init_um_state(um_state_t* state, const char* filename) {
  *state = (um_state_t) {
    .arrays = LIST_HEAD_INITIALIZER(state->arrays),
    .regs = { 0 },
    .operators = {
      conditional_move,
      array_index,
      array_amendment,
      addition,
      multiplication,
      division,
      not_and,
      halt,
      allocation,
      abandonment,
      output,
      input,
      load_program,
      load_value,
    },
    .execution_finger = 0,
  };
  file_array_t* zero_array = create_file_array(filename, 0);
  list_append(&zero_array->base.lnode, &state->arrays);
}

static inline size_t operator_num(platter cmd) {
  return (cmd >> 28) & 15;
}

void execution_step(um_state_t* state) {
  array_base_t* program = get_array(state, 0);
  platter cmd = array_get(program, state->execution_finger);
  ++state->execution_finger;
  state->operators[operator_num(cmd)](state, cmd);
}

int main(int argc, char **argv) {
  if (argc != 2) {
    fprintf(stderr, "Expected 1 argument: path to the file to interpret\n");
    return 1;
  }
  const char* filename = argv[1];

  um_state_t state;
  init_um_state(&state, filename);
  while (1) {
    execution_step(&state);
  }
}
