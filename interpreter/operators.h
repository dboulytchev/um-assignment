#ifndef _OPERATORS_H_
#define _OPERATORS_H_

#include "um_state.h"
#include "command.h"

void conditional_move(um_state_t* state, command_t cmd);

void array_index(um_state_t* state, command_t cmd);

void array_amendment(um_state_t* state, command_t cmd);

void addition(um_state_t* state, command_t cmd);

void multiplication(um_state_t* state, command_t cmd);

void division(um_state_t* state, command_t cmd);

void not_and(um_state_t* state, command_t cmd);

void halt(um_state_t* state, command_t cmd);

void allocation(um_state_t* state, command_t cmd);

void abandonment(um_state_t* state, command_t cmd);

void output(um_state_t* state, command_t cmd);

void input(um_state_t* state, command_t cmd);

void load_program(um_state_t* state, command_t cmd);

void orthography(um_state_t* state, command_t cmd);

typedef void (*operator_t)(um_state_t* state, command_t cmd);

#endif  // _OPERATORS_H_
