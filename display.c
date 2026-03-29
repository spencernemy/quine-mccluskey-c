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

void print_initial_groups() {

}

void print_final_expression(TermList final_implicants, int n) {
    char first_letter = 'A';
    printf("Final minimized Boolean expression: ");
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
