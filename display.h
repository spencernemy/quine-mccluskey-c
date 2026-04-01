#ifndef DISPLAY_H
#define DISPLAY_H

#include "qm.h"

void print_binary(unsigned int num, int bits);
void print_initial_input(const InputData * input);
void print_initial_groups(const TermList * groups, int n);
char * term_to_str(Term t, char * str, int n);
void print_combine_rounds_header();
void print_single_combine_round_header(int combine_rounds_completed);
void print_single_combination(Term term1, Term term2, Term new_term, int n);
void print_unused_terms_header(int combine_rounds_completed);
void print_unused_term(Term t, int n, int printed_prev_terms);
void print_end_combine_round_newlines(int printed_round_header, int printed_unused_header);
void print_prime_implicants(const TermList * prime_implicants, int n);
void print_prime_implicant_chart(const TermList * prime_implicants, const int * minterms, int minterm_count, int n);
void print_essential_prime_implicant(Term t, int covered_minterm, int * printed_header, int n);
void print_essential_section_end(int printed_header);
void print_remaining_minterms(const int * covered_cols, const int * remaining_minterms, int count);
void print_final_selected_implicants(const TermList * final_implicants, int n);
void print_final_expression(const TermList * final_implicants, int n);
void print_function_error(const char * function_name);

#endif