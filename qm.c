#include <stdio.h>
#include <stdlib.h>
#include "qm.h"

// Setup helper functions --------

int init_list(TermList * list, int initial_capacity) {
    if (!list || initial_capacity < 1) return 0;
    if (!(list->terms = malloc(sizeof(Term) * initial_capacity))) return 0;
    list->count = 0;
    list->capacity = initial_capacity;
    return 1;
}

void add_term(TermList * list, Term t) {
    if (!list) return;
    if (list->count >= list->capacity) {
        list->capacity *= 2;
        
    }
}

void free_list(TermList * list) {
    if (!list) return;
    free(list->terms);
    list->terms = NULL;
    list->count = 0;
    list->capacity = 0;
}

// End helper functions -----------


int build_initial_terms(TermList * list) {

}
