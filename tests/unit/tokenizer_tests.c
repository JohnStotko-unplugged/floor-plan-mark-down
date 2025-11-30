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
        return test_failed("expected token start 0");
    }

    if(tokenizer.currentToken.length != 4) {
        return test_failed("expected token length 4");
    }

    return test_passed();
}

int tokenizer_next_token_two_texts()
{
        report_test_start("tokenizer_next_token_two_texts");

    char *input = "one two";

    FILE* in = fmemopen(input, strlen(input), "r");

    struct FPMD_Tokenizer tokenizer;
    fpmd_tokenizer_init(&tokenizer, in);

    fpmd_tokenizer_next(&tokenizer);

    if(!fpmd_tokenizer_next(&tokenizer)) {
        return test_failed("expected to read next token");
    }

    if(tokenizer.currentToken.tokenType != TEXT) {
        return test_failed("expected token type TEXT");
    }

    if(tokenizer.currentToken.start != 4) {
        return test_failed("expected token start 4");
    }

    if(tokenizer.currentToken.length != 3) {
        return test_failed("expected token length 3");
    }

    return test_passed();
}

int tokenizer_next_token_number()
{
    report_test_start("tokenizer_next_token_number");
    return test_todo();
}

int tokenizer_next_token_a1_is_valid_text()
{
    report_test_start("tokenizer_next_token_a1_is_valid_text");
    return test_todo();
}

int tokenizer_next_token_1a_is_valid_text()
{
    report_test_start("tokenizer_next_token_1a_is_valid_text");
    return test_todo();
}

int tokenizer_next_token_quotes()
{
    report_test_start("tokenizer_next_token_quotes");
    return test_todo();
}

int tokenizer_next_token_indent()
{
    report_test_start("tokenizer_next_token_indent");
    return test_todo();
}

int tokenizer_next_token_newline()
{
    report_test_start("tokenizer_next_token_newline");
    return test_todo();
}

int tokenizer_next_token_multiple_indent()
{
    report_test_start("tokenizer_next_token_multiple_indent");
    return test_todo();
}

int tokenizer_next_token_multiple_newline()
{
    report_test_start("tokenizer_next_token_eof");
    return test_todo();
}

int main() {
    int result = TEST_SUCCESS;

    result |= tokenizer_initialization_test();
    result |= tokenizer_next_token_test();
    result |= tokenizer_next_token_two_texts();
    result |= tokenizer_next_token_number();
    result |= tokenizer_next_token_a1_is_valid_text();
    result |= tokenizer_next_token_1a_is_not_valid();
    result |= tokenizer_next_token_quotes();

    return result;
}