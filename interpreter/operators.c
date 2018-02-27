#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "operators.h"

void conditional_move(um_state_t* state, command_t cmd) {
  if (state->regs[cmd.c] != 0) {
    state->regs[cmd.a] = state->regs[cmd.b];
  }
}

void array_index(um_state_t* state, command_t cmd) {
  state->regs[cmd.a] = array_get(state, state->regs[cmd.b], state->regs[cmd.c]);
}

void array_amendment(um_state_t* state, command_t cmd) {
  array_set(state, state->regs[cmd.a], state->regs[cmd.b], state->regs[cmd.c]);
}

void addition(um_state_t* state, command_t cmd) {
  state->regs[cmd.a] = state->regs[cmd.b] + state->regs[cmd.c];
}

void multiplication(um_state_t* state, command_t cmd) {
  state->regs[cmd.a] = state->regs[cmd.b] * state->regs[cmd.c];
}

void division(um_state_t* state, command_t cmd) {
  state->regs[cmd.a] = state->regs[cmd.b] / state->regs[cmd.c];
}

void not_and(um_state_t* state, command_t cmd) {
  state->regs[cmd.a] = ~(state->regs[cmd.b] & state->regs[cmd.c]);
}

void halt(um_state_t* state, command_t cmd) {
  (void) state;
  (void) cmd;
  fflush(stdout);
  exit(0);
}

void allocation(um_state_t* state, command_t cmd) {
  state->regs[cmd.b] = (platter) allocate_ram_array(state->regs[cmd.c]);
}

void abandonment(um_state_t* state, command_t cmd) {
  free_ram_array(get_ram_array(state, state->regs[cmd.c]));
}

void output(um_state_t* state, command_t cmd) {
  assert(state->regs[cmd.c] < 255);
  printf("%c", (char) state->regs[cmd.c]);
  fflush(stdout);
}

void input(um_state_t* state, command_t cmd) {
  char ch;
  int scanf_res = scanf("%c", &ch);
  assert(scanf_res == EOF || scanf_res == 1);
  state->regs[cmd.c] = (scanf_res == EOF ? (~0) : ch);
}

void load_program(um_state_t* state, command_t cmd) {
  platter array_id = state->regs[cmd.b];
  if (array_id != 0) {
    ram_array_t* new = duplicate_ram_array(get_ram_array(state, array_id));
    free_ram_array(get_ram_array(state, 0));
    state->zero_array = new;
  }
  state->execution_finger = state->regs[cmd.c];
}

void orthography(um_state_t* state, command_t cmd) {
  state->regs[cmd.spec] = cmd.value;
}
