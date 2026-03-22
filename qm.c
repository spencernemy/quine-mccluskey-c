#include <stdio.h>
#include <stdlib.h>
#include "qm.h"
#include "parser.h"

// Setup helper functions --------

int init_list(TermList * list, int initial_capacity) {
    if (!list || initial_capacity < 1) return 0;
    if (!(list->terms = malloc(sizeof(Term) * initial_capacity))) return 0;
    list->count = 0;
    list->capacity = initial_capacity;
    return 1;
}

int add_term(TermList * list, Term t) {
    if (!list) return 0;
    if (list->count >= list->capacity) {
        list->capacity *= 2;
        list->terms = realloc(list->terms, sizeof(Term) * list->capacity);
    }
    
    list->terms[list->count++] = t;
    return 1;
}

void free_list(TermList * list) {
    if (!list) return;
    free(list->terms);
    list->terms = NULL;
    list->count = 0;
    list->capacity = 0;
}

// End helper functions -----------


int build_initial_terms(TermList * list, InputData * input) {
    if (!list || !input) return 0;
    Term t;
    for (int i = 0; i < input->count; i++) {
        t.value = input->minterms[i];
        t.mask = 0;
        t.used = 0;
        t.covers = NULL;
        t.cover_count = 0;
        if (!(add_term(list, t))) return 0;
    }
    return 1;
}

// Ran in main function
int run_qm_sequence(InputData * input) {
    if (!input) return 0;
    TermList * termList = malloc(sizeof(TermList));
    init_list(termList, input->count);
    build_initial_terms(termList, input);

    free(termList);
    return 1;
}