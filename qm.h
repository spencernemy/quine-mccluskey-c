#ifndef QM_H
#define QM_H

#include "parser.h"

typedef struct {
    unsigned int value; // Bit value (e.g. 010)
    unsigned int mask;  // Dash positions (1 = dash)
    int used;           // 1 if combined in a certain round
    int * covers;       // Array of minterms this term covers
    int cover_count;    // Number of minterms covered (in covers array)
} Term;

typedef struct {
    Term * terms;       // Array of terms
    int count;          // Number of terms (in terms array)
    int capacity;       // Capacity allocated to terms array
} TermList;

int init_list(TermList * list, int initial_capacity);
int add_term(TermList * list, Term t);
void free_list(TermList * list);
void free_group_views(TermList * groups, int n);
int build_initial_terms(TermList * list, InputData * input);
int build_single_term(Term * t, int value, int mask, int used, int cover_count);
TermList * group_minterms(TermList * current_terms, int n);
int combine_round(TermList * current_terms, TermList * next_terms, TermList * prime_implicants,
    int n, int combine_rounds_completed);
int can_combine(Term term1, Term term2);
int count_ones(unsigned int term);
int select_final_implicants(TermList * prime_implicants, int * initial_minterms,
    int initial_minterm_count, TermList * final_implicants, int n);
int all_cols_covered(int covered_cols[], int size);
int row_already_selected(int selected_rows[], int selected_row_count, int row_checked);
void print_initial_input(InputData * input);
void print_binary(unsigned int num, int bits);
void print_final_expression(TermList final_implicants, int n);
int run_qm_sequence(InputData * input);

#endif