#ifndef DISPLAY_H
#define DISPLAY_H

#include "qm.h"

void print_binary(unsigned int num, int bits);
void print_initial_input(InputData * input);
void print_initial_groups(TermList * groups, int n);
void print_single_combination(Term term1, Term term2, Term new_term, int n);
void print_unused_term(Term t, int n, int printed_prev_terms);
void print_prime_implicants(TermList prime_implicants, int n);
void print_final_expression(TermList final_implicants, int n);

#endif