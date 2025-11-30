#include <stdio.h>
#include <string.h>

#include "test.h"

#include "../../src/fpmd.h"


int tokenizer_initialization_test()
{
    report_test_start("tokenizer_initialization_test");

    struct FPMD_Tokenizer tokenizer;
    fpmd_tokenizer_init(&tokenizer, NULL);

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

int tokenizer_next_token_test()
{
    report_test_start("tokenizer_next_token_test");

    char *input = "line";

    FILE* in = fmemopen(input, strlen(input), "r");

    struct FPMD_Tokenizer tokenizer;
    fpmd_tokenizer_init(&tokenizer, in);

    if(!fpmd_tokenizer_next(&tokenizer)) {
        return test_failed("expected to read next token");
    }

    if(tokenizer.currentToken.tokenType != TEXT) {
        return test_failed("expected token type TEXT");
    }

    if(tokenizer.currentToken.start != 0) {
        printf("Got start: %d\n", tokenizer.currentToken.start);
        return test_failed("expected token start 0");
    }

    if(tokenizer.currentToken.length != 4) {
        return test_failed("expected token length 4");
    }

    return test_passed();
}

int main() {
    int result = TEST_SUCCESS;

    result |= tokenizer_initialization_test();
    result |= tokenizer_next_token_test();

    return result;
}