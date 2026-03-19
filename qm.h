#ifndef QM_H
#define QM_H

typedef struct {
    unsigned int value; // Bit value (e.g. 010)
    unsigned int mask;  // Dash positions (1 = dash)
    int used;           // 1 if combined in a certain round
    int * covers;       // Array of minterms this term covers
    int cover_count;    // Number of minterms covered (in covers array)
} Term;

typedef struct {
    Term * terms;       // Array of terms
    int count;          // Number of terms (in terms array)
    int capacity;       // Capacity allocated to terms array
} TermList;



#endif