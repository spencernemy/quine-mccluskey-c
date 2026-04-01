#include <stdio.h>
#include "display.h"

void print_binary(unsigned int num, int bits) {
    if (bits <= 0) return;
    for (int i = bits - 1; i >= 0; i--) {
        printf("%d", (num >> i) & 1);
    }
}

void print_initial_input(const InputData * input) {
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

void print_initial_groups(const TermList * groups, int n) {
    if (!groups) return;

    printf("Step 2: Initial groups\n");
    for (int i = 0; i < n + 1; i++) {
        printf("Group %d:\n", i);

        if (groups[i].count == 0) {
            printf("(None)");
        } else {
            for (int j = 0; j < groups[i].count; j++) {
                if (j != 0) printf(", ");
                print_binary(groups[i].terms[j].value, n);
            }
        }
        printf("\n");
    }
    printf("\n");
}

char * term_to_str(Term t, char * str, int n) {
    if (!str) return NULL;
    for (int i = n - 1; i >= 0; i--) {
        int value = (t.value >> i) & 1;
        int mask = (t.mask >> i) & 1;
        str[n - 1 - i] = mask ? '-' : (value ? '1' : '0');
    }
    str[n] = '\0';
    return str;
}

void print_combine_rounds_header() {
    printf("Step 3: Combine rounds\n\n");
}

void print_single_combine_round_header(int combine_rounds_completed) {
    printf("Combine Round %d\n", combine_rounds_completed + 1);
}

void print_single_combination(Term term1, Term term2, Term new_term, int n) {
    char term1_str[n+1];
    char term2_str[n+1];
    char new_term_str[n+1];
    printf("%s + %s -> %s\n", term_to_str(term1, term1_str, n), term_to_str(term2, term2_str, n), term_to_str(new_term, new_term_str, n));
}

void print_unused_terms_header(int combine_rounds_completed) {
    printf("Unused terms from round %d:\n", combine_rounds_completed + 1);
}

void print_unused_term(Term t, int n, int printed_prev_terms) {
    if (printed_prev_terms) printf(", ");
    char t_str[n+1];
    printf("%s", term_to_str(t, t_str, n));
}

void print_end_combine_round_newlines(int printed_round_header, int printed_unused_header) {
    if (printed_round_header) printf("\n");
    if (printed_unused_header) printf("\n");
}

void print_prime_implicants(const TermList * prime_implicants, int n) {
    if (!prime_implicants) return;

    printf("Step 4: Prime implicants\n");
    for (int i = 0; i < prime_implicants->count; i++) {
        char implicant_str[n+1];
        if (i != 0) printf(", ");
        printf("%s", term_to_str(prime_implicants->terms[i], implicant_str, n));
    }
    printf("\n\n");
}

void print_prime_implicant_chart(const TermList * prime_implicants, const int * minterms, int minterm_count, int n) {
    if (!prime_implicants || !minterms) return;
    
    int margin_width = n + 5;

    printf("Step 5: Prime implicant chart\n\n");
    printf("%-*s", margin_width, "");
    for (int i = 0; i < minterm_count; i++) {
        printf("%-3d", minterms[i]);
    }
    printf("\n");

    for (int i = 0; i < prime_implicants->count; i++) {
        char implicant_str[n+1];
        printf("%-*s", margin_width, term_to_str(prime_implicants->terms[i], implicant_str, n));

        for (int j = 0; j < minterm_count; j++) {
            int covers = 0;
            for (int k = 0; k < prime_implicants->terms[i].cover_count; k++) {
                if (prime_implicants->terms[i].covers[k] == minterms[j]) {
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

void print_essential_prime_implicant(Term t, int covered_minterm, int * printed_header, int n) {
    if (!printed_header) return;
    if (!(*printed_header)) {
        printf("Step 6: Essential prime implicants\n");
        *printed_header = 1;
    }

    char implicant_str[n+1];
    printf("%s is essential (covers minterm %d only)\n", term_to_str(t, implicant_str, n), covered_minterm);

}

void print_remaining_minterms(const int * covered_cols, const int * remaining_minterms, int count) {
    if (!covered_cols || !remaining_minterms) return;
    printf("Step 7: Remaining uncovered minterms\n");

    int printed = 0;
    for (int i = 0; i < count; i++) {
        if (covered_cols[i]) continue;

        if (printed) printf(", ");
        printf("%d", remaining_minterms[i]);
        printed = 1;
    }

    if (!printed) printf("(None)");

    printf("\n\n");
}

void print_final_selected_implicants(const TermList * final_implicants, int n) {
    if (!final_implicants) return;
    printf("Step 8: Final selected implicants\n");

    if (final_implicants->count == 0) {
        printf("(None)\n\n");
        return;
    }

    char implicant_str[n+1];
    for (int i = 0; i < final_implicants->count; i++) {
        if (i != 0) printf(", ");
        printf("%s", term_to_str(final_implicants->terms[i], implicant_str, n));
    }
    printf("\n\n");
}

void print_final_expression(const TermList * final_implicants, int n) {
    if (!final_implicants) return;
    
    char first_letter = 'A';
    printf("Final minimized Boolean expression = ");
    for (int i = 0; i < final_implicants->count; i++) {
        if (i != 0) printf(" + ");

        int printed_literal = 0;

        Term t = final_implicants->terms[i];
        for (int j = 0; j < n; j++) {
            int bit_position = n - 1 - j;
            int bit = (t.value >> bit_position) & 1;
            int is_masked = (t.mask >> bit_position) & 1;

            if (is_masked) continue;
            printed_literal = 1;

            printf("%c", first_letter + j);
            if (bit == 0) printf("'");
        }
        
        // Covers case where minterm is all dashes (ex. ----)
        if (!printed_literal) printf("1");
    }
    printf("\n");
}

void print_function_error(const char * function_name) {
    printf("Error: %s failed.\n", function_name);
}