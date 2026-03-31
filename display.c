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

char * term_to_str(Term t, char * str, int n) {
    for (int i = n - 1; i >= 0; i--) {
        int value = (t.value >> i) & 1;
        int mask = (t.mask >> i) & 1;
        str[n - 1 - i] = mask ? '-' : (value ? '1' : '0');
    }
    str[n] = '\0';
    return str;
}

void print_single_combination(Term term1, Term term2, Term new_term, int n) {
    char term1_str[n+1];
    char term2_str[n+1];
    char new_term_str[n+1];
    printf("%s + %s -> %s\n", term_to_str(term1, term1_str, n), term_to_str(term2, term2_str, n), term_to_str(new_term, new_term_str, n));
}

void print_unused_term(Term t, int n, int printed_prev_terms) {
    if (printed_prev_terms) printf(", ");
    char t_str[n+1];
    printf("%s", term_to_str(t, t_str, n));
}

void print_prime_implicants(TermList prime_implicants, int n) {
    printf("Step 4: Prime implicants\n");
    for (int i = 0; i < prime_implicants.count; i++) {
        char implicant_str[n+1];
        if (i != 0) printf(", ");
        printf("%s", term_to_str(prime_implicants.terms[i], implicant_str, n));
    }
    printf("\n\n");
}

void print_prime_implicant_chart(TermList prime_implicants, int * minterms, int minterm_count, int n) {
    if (!minterms) return;
    
    int margin_width = n + 5;

    printf("Step 5: Prime implicant chart\n\n");
    printf("%-*s", margin_width, "");
    for (int i = 0; i < minterm_count; i++) {
        printf("%-3d", minterms[i]);
    }
    printf("\n");

    for (int i = 0; i < prime_implicants.count; i++) {
        char implicant_str[n+1];
        printf("%-*s", margin_width, term_to_str(prime_implicants.terms[i], implicant_str, n));

        for (int j = 0; j < minterm_count; j++) {
            int covers = 0;
            for (int k = 0; k < prime_implicants.terms[i].cover_count; k++) {
                if (prime_implicants.terms[i].covers[k] == minterms[j]) {
                    covers = 1;
                    break;
                }
            }
            printf("%-3s", covers ? "X" : "");
        }
        printf("\n");
    }
    printf("\n");
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
