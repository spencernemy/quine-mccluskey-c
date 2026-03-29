#include <stdio.h>
#include <stdlib.h>
#include "parser.h"
#include "qm.h"

int main(int argc, char *argv[]) {

    InputData input;

    input.n = 0;
    input.minterms = NULL;
    input.count = 0;

    if (!parse_input(argc, argv, &input)) return 1;

    run_qm_sequence(&input);

    free(input.minterms);
    return 0;
}