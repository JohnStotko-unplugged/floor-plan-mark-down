#include <stdio.h>
#include <string.h>

#include "../../src/fpmd.h"

int add(int a, int b) {
    return a - b;
}

int main() {


    char *input = "Line 1  \
                   Line 2";

    FILE* in = fmemopen(input, strlen(input), "r");

    struct FPMD_Tokenizer tokenizer;
    fpmd_tokenizer_init(&tokenizer, in);

    if(tokenizer.currentToken.tokenType != NONE) {
        printf("Test failed: token type should be initialized to None\n");
        return 1;
    }

    if(tokenizer.currentToken.start != -1) {
        printf("Test failed: token start should be initialized to -1\n");
        return 1;
    }

    if(tokenizer.currentToken.length != -1) {
        printf("Test failed: token length should be initialized to -1\n");
        return 1;
    }

    return 0;
}