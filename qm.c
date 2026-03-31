#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "qm.h"
#include "display.h"

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

void free_group_views(TermList * groups, int n) {
    if (!groups) return;
    for (int i = 0; i < n + 1; i++) {
        free(groups[i].terms);
    }
    free(groups);
}

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

int build_single_term(Term * t, int value, int mask, int used, int cover_count) {
    if (!t) return 0;
    t->value = value;
    t->mask = mask;
    t->used = used;
    t->cover_count = cover_count;
    if (!(t->covers = malloc(sizeof(int) * cover_count))) return 0;
    return 1;
}

/* group_minterms:
    Takes the original list of terms, creates (n + 1) new lists of terms based on
    number of '1's in each minterm to group them, and places each minterm into
    the group it belongs in.
*/
TermList * group_minterms(TermList * current_terms, int n) {
    if (!current_terms || n < 0) return NULL;

    int group_count = n + 1;

    // Array of TermLists
    TermList * groups;
    if (!(groups = malloc(sizeof(TermList) * group_count))) return NULL;
    
    // Initialize each group
    for (int i = 0; i < group_count; i++) {
        if(!init_list(&groups[i], 1)) {
            for (int j = 0; j < i; j++) {
                free(groups[j].terms);
            }
            free(groups);
            return NULL;
        }
    }

    // Place each term into its correct group
    for (int i = 0; i < current_terms->count; i++) {
        Term t = current_terms->terms[i];
        
        int ones = count_ones(t.value & ~t.mask);

        if (ones < 0 || ones > n || !add_term(&groups[ones], t)) {
            for (int j = 0; j < group_count; j++) {
                free(groups[j].terms);
            }
            free(groups);
            return NULL;
        }
    }

    return groups;
}

int combine_round(TermList * current_terms, TermList * next_terms, TermList * prime_implicants,
    int n, int combine_rounds_completed) {
    if (!current_terms || !next_terms || !prime_implicants || n < 0) return 0;

    int printed_round_header = 0;

    TermList * groups = group_minterms(current_terms, n);
    if (!groups) return 0;

    if (combine_rounds_completed == 0) {
        print_initial_groups(groups, n);
        printf("Step 3: Combine rounds\n\n");
    }

    // Reset used to 0 for all terms
    for (int i = 0; i < current_terms->count; i++) {
        current_terms->terms[i].used = 0;
    }

    // Loop through each comparison needed (groups[0] w/ groups[1], groups[1] w/ groups[2], etc.)
    for (int i = 0; i < n; i++) {
        TermList group1 = groups[i];
        TermList group2 = groups[i + 1];

        // Loop through each minterm in group1
        for (int j = 0; j < group1.count; j++) {
            Term * term1 = &group1.terms[j];
            
            // Loop through each minterm in group2 to compare with group1
            for (int k = 0; k < group2.count; k++) {
                Term * term2 = &group2.terms[k];

                if (!can_combine(*term1, *term2)) continue;

                if (!printed_round_header) {
                    printf("Combine Round %d\n", combine_rounds_completed + 1);
                    printed_round_header = 1;
                }

                for (int l = 0; l < current_terms->count; l++) {
                    if (current_terms->terms[l].covers == term1->covers)
                        current_terms->terms[l].used = 1;
                    if (current_terms->terms[l].covers == term2->covers)
                        current_terms->terms[l].used = 1;
                }

                int diff = term1->value ^ term2->value; // XOR
                int new_value = term1->value & ~diff; // Clears diff bit since it's now masked
                int new_mask = term1->mask | diff; // Add diff bit to mask
                int used = 0;
                int new_cover_count = term1->cover_count + term2->cover_count;
                
                Term new_term;
                if (!build_single_term(&new_term, new_value, new_mask, used, new_cover_count)) {
                    free_group_views(groups, n);
                    return 0;
                }

                // Copy covers
                for (int a = 0; a < term1->cover_count; a++) {
                    new_term.covers[a] = term1->covers[a];
                }
                for (int b = 0; b < term2->cover_count; b++) {
                    new_term.covers[term1->cover_count + b] = term2->covers[b];
                }

                // Duplicate check
                int duplicate = 0;
                for (int m = 0; m < next_terms->count; m++) {
                    if (new_term.value == next_terms->terms[m].value &&
                        new_term.mask == next_terms->terms[m].mask) {
                        duplicate = 1;
                        break;
                    }
                }
                if (duplicate) {
                    free(new_term.covers);
                    continue;
                }

                print_single_combination(*term1, *term2, new_term, n);

                if (!add_term(next_terms, new_term)) {
                    free(new_term.covers);
                    free_group_views(groups, n);
                    return 0;
                }
            }
        }
    }

    // Move unused original terms into prime_implicants
    int printed_unused_header = 0;
    int printed_prev_terms = 0;
    for (int i = 0; i < current_terms->count; i++) {
        
        if (current_terms->terms[i].used) continue;

        if (printed_round_header && !printed_unused_header) {
            printf("Unused terms from round %d:\n", combine_rounds_completed + 1);
            printed_unused_header = 1;
        }
        
        Term t;
        if (!(build_single_term(&t, current_terms->terms[i].value, current_terms->terms[i].mask,
            current_terms->terms[i].used, current_terms->terms[i].cover_count))) {
            free_group_views(groups, n);
            return 0;
        }

        for (int j = 0; j < t.cover_count; j++) {
            t.covers[j] = current_terms->terms[i].covers[j];
        }

        if (!(add_term(prime_implicants, t))) {
            free(t.covers);
            free_group_views(groups, n);
            return 0;
        }

        if (printed_round_header) print_unused_term(t, n, printed_prev_terms++);
    }

    if (printed_round_header) printf("\n");
    if (printed_unused_header) printf("\n");

    free_group_views(groups, n);
    return 1;
}

int can_combine(Term term1, Term term2) {
    if (term1.mask != term2.mask) return 0;
    int diff = term1.value ^ term2.value; // term1 XOR term2 (bits that differ become 1)
    // If difference > 0 AND only one bit difference
    if (diff && !(diff & (diff - 1))) {
        return 1;
    }
    return 0;
}

int count_ones(unsigned int term) {
    int count = 0;
    while (term) {
        term = term & (term - 1);
        count++;
    }
    return count;
}

int select_final_implicants(TermList * prime_implicants, int * initial_minterms,
    int initial_minterm_count, TermList * final_implicants) {
    if (!prime_implicants || !initial_minterms) return 0;
    
    int rows = prime_implicants->count;
    int cols = initial_minterm_count;
    int pi_table[rows][cols]; // Prime implicants table
    
    int col_one_counts[cols]; // Array storing the amount of 1s in each column (needed for a later step)
    memset(col_one_counts, 0, sizeof(col_one_counts)); // Initializes all elements to 0

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            pi_table[i][j] = 0;
            for (int k = 0; k < prime_implicants->terms[i].cover_count; k++) {
                if (prime_implicants->terms[i].covers[k] == initial_minterms[j]) {
                    pi_table[i][j] = 1;
                    col_one_counts[j]++;
                }
            }
        }
    }
    
    int selected_rows[rows];
    int selected_row_count = 0;
    int covered_cols[cols];
    memset(selected_rows, 0, sizeof(selected_rows));
    memset(covered_cols, 0, sizeof(covered_cols));

    for (int j = 0; j < cols; j++) {
        if (col_one_counts[j] != 1) continue;

        // Identify essential minterms
        int essential_row = -1;
        for (int i = 0; i < rows; i++) {
            if (pi_table[i][j] == 1) {
                essential_row = i;
                break;
            }
        }
        if (essential_row == -1) continue;

        if (row_already_selected(selected_rows, selected_row_count, essential_row)) continue;

        selected_rows[selected_row_count++] = essential_row;

        // Marks column (minterm) as covered/done
        for (int k = 0; k < cols; k++) {
            if (pi_table[essential_row][k] == 1) {
                covered_cols[k] = 1;
            }
        }
    }
    

    // Greedy method to cover remaining minterms
    while (!all_cols_covered(covered_cols, cols)) {
        int best_row = -1;
        int best_cover_count = 0;

        for (int i = 0; i < rows; i++) {
            if (row_already_selected(selected_rows, selected_row_count, i)) continue;

            int current_cover_count = 0;
            for (int j = 0; j < cols; j++) {
                if (pi_table[i][j] == 1 && covered_cols[j] == 0) current_cover_count++;
            }
            if (current_cover_count > best_cover_count) {
                best_row = i;
                best_cover_count = current_cover_count;
            }
        }

        if (best_row == -1) return 0;

        selected_rows[selected_row_count++] = best_row;

        for (int k = 0; k < cols; k++) {
            if (pi_table[best_row][k] == 1) {
                covered_cols[k] = 1;
            }
        }
    }

    if (!init_list(final_implicants, selected_row_count)) return 0;

    // Deep copy terms to final implicants
    for (int i = 0; i < selected_row_count; i++) {
        int row = selected_rows[i];
        Term pi_term = prime_implicants->terms[row];

        Term t;
        if (!build_single_term(&t, pi_term.value, pi_term.mask, 0, pi_term.cover_count)) {
            free_list(final_implicants);
            return 0;
        }

        for (int j = 0; j < pi_term.cover_count; j++) {
            t.covers[j] = pi_term.covers[j];
        }

        if (!add_term(final_implicants, t)) {
            free(t.covers);
            free_list(final_implicants);
            return 0;
        }
    }

    return 1;
}

int all_cols_covered(int covered_cols[], int size) {
    for (int i = 0; i < size; i++) {
        if (covered_cols[i] == 0) return 0;
    }
    return 1;
}

int row_already_selected(int selected_rows[], int selected_row_count, int row_checked) {
    for (int i = 0; i < selected_row_count; i++) {
        if (selected_rows[i] == row_checked) {
            return 1;
        }
    }
    return 0;
}

// Ran in main function
int run_qm_sequence(InputData * input) {
    if (!input) return 0;
    
    int success = 1;
    int n = input->n;

    print_initial_input(input);

    // Temporarily initialized (prevents freeing uninitialized)
    TermList initial_list = {0};
    TermList * current_terms = &initial_list;
    TermList next_terms = {0};
    TermList prime_implicants = {0};
    TermList final_implicants = {0};

    if (!init_list(&initial_list, input->count)) {
        printf("Error: init_list failed.\n");
        success = 0; goto cleanup;
    }

    if (!build_initial_terms(&initial_list, input)) {
        printf("Error: build_initial_terms failed.\n");
        success = 0; goto cleanup;
    }

    if (!init_list(&next_terms, input->count) || !init_list(&prime_implicants, input->count)) {
        printf("Error: init_list failed.\n");
        success = 0; goto cleanup;
    }
    
    int combine_rounds_completed = 0;
    while (1) {
        if (!combine_round(current_terms, &next_terms, &prime_implicants, n, combine_rounds_completed)) {
            printf("Error: combine_round failed.\n");
            success = 0; goto cleanup;
        }

        combine_rounds_completed++;
        
        if (next_terms.count == 0) break;

        free_list(current_terms);
        *current_terms = next_terms;
        
        if (!init_list(&next_terms, input->count)) {
            printf("Error: init_list failed.\n");
            success = 0; goto cleanup;
        }
    }

    print_prime_implicants(prime_implicants, n);
    print_prime_implicant_chart(prime_implicants, input->minterms, input->count, n);

    if (!select_final_implicants(&prime_implicants, input->minterms, input->count, &final_implicants)) {
        printf("Error: select_final_implicants failed.\n");
        success = 0; goto cleanup;
    }

    print_final_expression(final_implicants, n);

cleanup:
    free_list(&final_implicants);
    free_list(&next_terms);
    free_list(&prime_implicants);
    free_list(&initial_list);

    return success;
}