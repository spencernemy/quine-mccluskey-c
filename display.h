#ifndef DISPLAY_H
#define DISPLAY_H

#include "qm.h"

void print_binary(unsigned int num, int bits);
void print_initial_input(InputData * input);
void print_initial_groups();
void print_final_expression(TermList final_implicants, int n);

#endif