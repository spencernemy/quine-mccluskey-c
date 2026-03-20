#include <stdio.h>
#include <stdlib.h>
#include "qm.h"

int init_list(TermList * list, int initial_capacity) {
    if (initial_capacity < 1) return 0;
    if (!(list->terms = malloc(sizeof(Term) * initial_capacity))) return 0;
    list->count = 0;
    list->capacity = initial_capacity;
    return 1;
}

void add_term(TermList * list, Term t) {
    if (list->count >= list->capacity) {
        list->capacity *= 2;
        
    }
}