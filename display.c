#include <stdio.h>
#include <stdlib.h>
#include "display.h"

void print_binary(unsigned int num, int bits) {
    for (int i = bits - 1; i >= 0; i--) {
        printf("%d", (num >> i) & 1);
    }
}

void print_initial_input(InputData * input) {
    if (!input) return;

    printf("Variables: %d\n", input->n);
    printf("Minterms: ");
    for (int i = 0; i < input->count; i++) {
        printf("%d ", input->minterms[i]);
    }
    printf("\n\n");

    printf("Step 1: Binary terms\n");
    for (int i = 0; i < input->count; i++) {
        if (i != 0) printf(", ");
        print_binary(input->minterms[i], input->n);
    }
    printf("\n\n");
}

void print_initial_groups(TermList * groups, int n) {
    printf("Step 2: Initial groups\n");
    for (int i = 0; i < n + 1; i++) {
        printf("Group %d:\n", i);
        int j;
        for (j = 0; j < groups[i].count; j++) {
            if (j != 0) printf(", ");
            print_binary(groups[i].terms[j].value, n);
        }
        if (j == 0) printf("(None)");
        printf("\n");
    }
    printf("\n");
}

void print_combine_round(TermList current_terms, TermList next_terms, int combine_rounds_completed) {
    if (combine_rounds_completed == 1) printf("Step 3: Combine rounds\n");
    
}

void print_final_expression(TermList final_implicants, int n) {
    char first_letter = 'A';
    printf("Final minimized Boolean expression = ");
    for (int i = 0; i < final_implicants.count; i++) {
        if (i != 0) printf(" + ");

        Term t = final_implicants.terms[i];
        for (int j = 0; j < n; j++) {
            int bit_position = n - 1 - j;
            int bit = (t.value >> bit_position) & 1;
            int is_masked = (t.mask >> bit_position) & 1;

            if (is_masked) continue;

            printf("%c", first_letter + j);
            if (bit == 0) printf("'");
        }
    }
    printf("\n");
}
