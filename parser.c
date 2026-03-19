#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <errno.h>
#include "parser.h"

int parse_input(int argc, char *argv[], InputData * data) {
    if (argc < 3) {
        printf("Error: Invalid input(s).\n");
        return 0;
    }

    if(!(data->minterms = malloc(sizeof(int) * (argc-2)))) {
        printf("Error: Memory allocation failed.\n");
        return 0;
    }

    for (int i = 1; i < argc; i++) {
        char *end;
        errno = 0;
        long val = strtol(argv[i], &end, 10);
        if (errno == ERANGE || end == argv[i] || *end != '\0' || val < 0 || val > INT_MAX) {
            printf("Error: Invalid input(s).\n");
            cleanup_input(data);
            return 0;
        }

        if (i == 1) { // first argument after program name = num of variables
            if ((int)val < 1 || (int)val > 6) {
                printf("Error: Supported variable count is between 1-6.\nMaximum supported variables is 6 due to performance limits.\n");
                cleanup_input(data);
                return 0;
            }
            data->n = (int)val;
        } else { // All minterms
            data->minterms[i-2] = (int)val;
        }
    }

    data->count = argc - 2;

    // Sort minterm array
    qsort(data->minterms, data->count, sizeof(int), comp);
    data->count = remove_duplicates(data);
    
    if (!(validate_minterms(data))) {
        printf("Error: One or more minterms are invalid.\n");
        cleanup_input(data);
        return 0;
    }

    return 1;
}

int comp(const void *a, const void *b) {
    const int *x = (const int *)a;
    const int *y = (const int *)b;

    if (*x < *y) return -1;
    if (*x > *y) return 1;
    return 0;
}

void cleanup_input(InputData * data) {
    free(data->minterms);
    data->minterms = NULL;
    data->count = 0;
}

// Returns new array length
int remove_duplicates(InputData * data) {
    if (data->count == 0) return 0;
    int * arr = data->minterms;

    int write = 1; // next place to put a new unique value
    
    for (int read = 1; read < data->count; read++) {
        if (arr[read] != arr[read - 1]) {
            arr[write] = arr[read];
            write++;
        }
    }

    return write;
}

int validate_minterms(InputData * data) {
    for (int i = 0; i < data->count; i++) {
        if (data->minterms[i] >= (1 << data->n)) { // computes 2^n using bit shifting
            return 0;
        }
    }
    return 1;
}
