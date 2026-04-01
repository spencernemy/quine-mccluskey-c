#ifndef DISPLAY_H
#define DISPLAY_H

#include "qm.h"

void print_binary(unsigned int num, int bits);
void print_initial_input(InputData * input);
void print_initial_groups(TermList * groups, int n);
char * term_to_str(Term t, char * str, int n);
void print_combine_rounds_header();
void print_single_combine_round_header(int combine_rounds_completed);
void print_single_combination(Term term1, Term term2, Term new_term, int n);
void print_unused_terms_header(int combine_rounds_completed);
void print_unused_term(Term t, int n, int printed_prev_terms);
void print_end_combine_round_newlines(int printed_round_header, int printed_unused_header);
void print_prime_implicants(TermList prime_implicants, int n);
void print_prime_implicant_chart(TermList prime_implicants, int * minterms, int minterm_count, int n);
void print_essential_prime_implicant(int * printed_header);
void print_final_expression(TermList final_implicants, int n);
void print_error(char * error_function_name);

#endif