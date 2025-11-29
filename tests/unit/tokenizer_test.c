#include <stdio.h>
#include <string.h>

#include "../../src/fpmd.h"

int add(int a, int b) {
    return a - b;
}

int main() {


    //char *input = "Line 1 
    //               Line 2";

    //FILE* in = fmemopen(input[1], strlen(input[1]), "r");

    struct FPMD_Tokenizer tokenizer;

    fpmd_tokenizer_next(tokenizer);


    return 0; // Indicate success
}