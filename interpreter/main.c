#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "operators.h"


static const operator_t OPERATORS[14] = {
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
  orthography,
};

void execution_step(um_state_t* state) {
  command_t cmd = { .raw = array_get(state, 0, state->execution_finger) };
  ++state->execution_finger;
  OPERATORS[cmd.op](state, cmd);
}

int main(int argc, const char** argv) {
  if (argc != 2) {
    fprintf(stderr, "Expected 1 argument: filename\n");
    exit(2);
  }
  ram_array_t* program = ram_array_from_file(argv[1]);
  um_state_t state = UM_STATE_INIT(program);

  while (true) {
    execution_step(&state);
  }
}
