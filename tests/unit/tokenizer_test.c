#include <stdio.h>
#include <string.h>

#include "test.h"

#include "../../src/fpmd.h"


int tokenizer_initialization_test()
{
    report_test_start("tokenizer_initialization_test");

    char *input = "Line 1  \
                Line 2";

    FILE* in = fmemopen(input, strlen(input), "r");

    struct FPMD_Tokenizer tokenizer;
    fpmd_tokenizer_init(&tokenizer, in);

    if(tokenizer.currentToken.tokenType != NONE) {
        return test_failed("token type should be initialized to None");
    }

    if(tokenizer.currentToken.start != -1) {
        return test_failed("token start should be initialized to -1");
    }

    if(tokenizer.currentToken.length != -1) {
        return test_failed("token length should be initialized to -1");
    }

    return test_passed();
}

int main() {
    int result = TEST_SUCCESS;

    result |= tokenizer_initialization_test();

    return result;
}