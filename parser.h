#ifndef PARSER_H
#define PARSER_H

typedef struct {
    int n; // Num of variables in QM
    int * minterms; // Array of minterms
    int count; // Count of minterm inputs
} InputData;

int parse_input(int argc, char *argv[], InputData * data);
int comp(const void *a, const void *b);

#endif