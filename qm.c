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
    if (!list || !(list->terms) || list->capacity <= 0) return 0;
    if (list->count >= list->capacity) {
        int newCapacity = list->capacity * 2;

        Term * temp;
        if (!(temp = realloc(list->terms, sizeof(Term) * newCapacity))) return 0;
        list->capacity = newCapacity;
        list->terms = temp;
    }
    
    list->terms[list->count++] = t;
    return 1;
}

void free_list(TermList * list) {
    if (!list || !(list->terms)) return;

    for (int i = 0; i < list->count; i++) {
        free(list->terms[i].covers);
    }

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
        if (!(t.covers = malloc(sizeof(int)))) return 0;
        t.covers[0] = input->minterms[i];
        t.cover_count = 1;
        if (!add_term(list, t)) {
            free(t.covers);
            return 0;
        }
    }
    return 1;
}

/* group_minterms:
    Takes the original list of terms, creates n + 1 new lists of terms based on
    number of '1's in each minterm to group them, and places each minterm into
    the group it belongs in.
*/
int group_minterms(TermList * current_terms, InputData * input) {
    if (!current_terms) return 0;

    int group_count = input->n + 1;
    TermList groups[group_count]; // Array of TermLists
    for (int i = 0; i < group_count; i++) {
        init_list(&groups[i], 1);
    }

    for (int i = 0; i < current_terms->count; i++) {
        Term t = current_terms->terms[i];
        int ones = count_ones(t);
        add_term(&groups[ones], t);
    }


    return 1;
}

int count_ones(Term t) {
    int val = t.value;
    int count = 0;
    while (val) {
        val = val & (val - 1);
        count++;
    }
    return count;
}

// Ran in main function
int run_qm_sequence(InputData * input) {
    if (!input) return 0;
    TermList termList;

    if (!init_list(&termList, input->count)) {
        printf("Error: QM sequence failed.\n");
        return 0;
    }

    if (!build_initial_terms(&termList, input)) {
        printf("Error: QM sequence failed.\n");
        free_list(&termList);
        return 0;
    }

    if (!group_minterms(&termList, input)) {
        printf("Error: QM sequence failed.\n");
        free_list(&termList);
        return 0;
    }

    free_list(&termList);
    return 1;
}