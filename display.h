#ifndef DISPLAY_H
#define DISPLAY_H

#include "qm.h"

void print_binary(unsigned int num, int bits);
void print_initial_input(InputData * input);
void print_initial_groups(TermList * groups, int n);
void print_combine_round(TermList current_terms, TermList next_terms, int combine_rounds_completed);
void print_final_expression(TermList final_implicants, int n);

#endif