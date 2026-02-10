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

    if(tokenizer.currentToken.length != 0) {
        return test_failed("token length should be initialized to 0");
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

    int ret = fpmd_tokenizer_next(&tokenizer);
    if(ret != 1) {
        char msg[256];
        sprintf(msg, "expected to read next token, but got error code %d", ret);
        return test_failed(msg);
    }

    if(tokenizer.currentToken.tokenType != TEXT) {
        return test_failed("expected token type TEXT");
    }

    int bufferSize = fpmp_token_buffersize(&tokenizer);
    char buffer[bufferSize];

    fpmd_token_value(&tokenizer, buffer, bufferSize);

    if(strcmp(buffer, "line") != 0) {
        char msg[256];
        sprintf(msg, "expected token value 'line', but got '%s'", buffer);
        return test_failed(msg);
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

    int ret = fpmd_tokenizer_next(&tokenizer);

    if(ret != 1) {
        char msg[256];
        sprintf(msg, "expected to read next token, but got error code %d", ret);
        return test_failed(msg);
    }

    int bufferSize = fpmp_token_buffersize(&tokenizer);
    char buffer[bufferSize];

    fpmd_token_value(&tokenizer, buffer, bufferSize);

    if(strcmp(buffer, "two") != 0) {
        char msg[256];
        sprintf(msg, "expected token value 'two', but got '%s'", buffer);
        return test_failed(msg);
    }

    return test_passed();
}

int tokenizer_next_token_two_texts_space()
{
    report_test_start("tokenizer_next_token_two_texts_space");

    char *input = "one    two";
    FILE* in = fmemopen(input, strlen(input), "r");

    struct FPMD_Tokenizer tokenizer;
    fpmd_tokenizer_init(&tokenizer, in);

    fpmd_tokenizer_next(&tokenizer);

    int ret = fpmd_tokenizer_next(&tokenizer);

    if(ret != 1) {
        char msg[256];
        sprintf(msg, "expected to read next token, but got error code %d", ret);
        return test_failed(msg);
    }

    int bufferSize = fpmp_token_buffersize(&tokenizer);
    char buffer[bufferSize];

    fpmd_token_value(&tokenizer, buffer, bufferSize);

    if(strcmp(buffer, "two") != 0) {
        char msg[256];
        sprintf(msg, "expected token value 'two', but got '%s'", buffer);
        return test_failed(msg);
    }

    return test_passed();
}

int tokenizer_next_token_number()
{
    report_test_start("tokenizer_next_token_number");

    char *input = "123";
    FILE* in = fmemopen(input, strlen(input), "r");

    struct FPMD_Tokenizer tokenizer;
    fpmd_tokenizer_init(&tokenizer, in);

    int ret = fpmd_tokenizer_next(&tokenizer);
    if(ret != 1) {
        char msg[256];
        sprintf(msg, "expected to read next token, but got error code %d", ret);
        return test_failed(msg);
    }

    if(tokenizer.currentToken.tokenType != TEXT) {
        return test_failed("expected token type TEXT");
    }

    int bufferSize = fpmp_token_buffersize(&tokenizer);
    char buffer[bufferSize];

    fpmd_token_value(&tokenizer, buffer, bufferSize);

    if(strcmp(buffer, "123") != 0) {
        char msg[256];
        sprintf(msg, "expected token value '123', but got '%s'", buffer);
        return test_failed(msg);
    }

    return test_passed();
}

int tokenizer_next_token_a1_is_valid_text()
{
    report_test_start("tokenizer_next_token_a1_is_valid_text");

    char *input = "a1";
    FILE* in = fmemopen(input, strlen(input), "r");

    struct FPMD_Tokenizer tokenizer;
    fpmd_tokenizer_init(&tokenizer, in);

    int ret = fpmd_tokenizer_next(&tokenizer);
    if(ret != 1) {
        char msg[256];
        sprintf(msg, "expected to read next token, but got error code %d", ret);
        return test_failed(msg);
    }

    if(tokenizer.currentToken.tokenType != TEXT) {
        return test_failed("expected token type TEXT");
    }

    int bufferSize = fpmp_token_buffersize(&tokenizer);
    char buffer[bufferSize];

    fpmd_token_value(&tokenizer, buffer, bufferSize);

    if(strcmp(buffer, "a1") != 0) {
        char msg[256];
        sprintf(msg, "expected token value 'a1', but got '%s'", buffer);
        return test_failed(msg);
    }

    return test_passed();
}

int tokenizer_next_token_1a_is_valid_text()
{
    report_test_start("tokenizer_next_token_1a_is_valid_text");

    char *input = "1a";
    FILE* in = fmemopen(input, strlen(input), "r");

    struct FPMD_Tokenizer tokenizer;
    fpmd_tokenizer_init(&tokenizer, in);

    int ret = fpmd_tokenizer_next(&tokenizer);
    if(ret != 1) {
        char msg[256];
        sprintf(msg, "expected to read next token, but got error code %d", ret);
        return test_failed(msg);
    }

    if(tokenizer.currentToken.tokenType != TEXT) {
        return test_failed("expected token type TEXT");
    }

    int bufferSize = fpmp_token_buffersize(&tokenizer);
    char buffer[bufferSize];

    fpmd_token_value(&tokenizer, buffer, bufferSize);

    if(strcmp(buffer, "1a") != 0) {
        char msg[256];
        sprintf(msg, "expected token value '1a', but got '%s'", buffer);
        return test_failed(msg);
    }

    return test_passed();
}

int tokenizer_next_token_quotes()
{
    report_test_start("tokenizer_next_token_quotes");

    char *input = "'I like spaces'";
    FILE* in = fmemopen(input, strlen(input), "r");

    struct FPMD_Tokenizer tokenizer;
    fpmd_tokenizer_init(&tokenizer, in);

    int ret = fpmd_tokenizer_next(&tokenizer);
    if(ret != 1) {
        char msg[256];
        sprintf(msg, "expected to read next token, but got error code %d", ret);
        return test_failed(msg);
    }

    if(tokenizer.currentToken.tokenType != TEXT) {
        return test_failed("expected token type TEXT");
    }

    int bufferSize = fpmp_token_buffersize(&tokenizer);
    char buffer[bufferSize];

    fpmd_token_value(&tokenizer, buffer, bufferSize);

    if(strcmp(buffer, "I like spaces") != 0) {
        char msg[256];
        sprintf(msg, "expected token value 'I like spaces', but got '%s'", buffer);
        return test_failed(msg);
    }

    return test_passed();
}

int tokenizer_next_token_indent()
{
    report_test_start("tokenizer_next_token_indent");

    char *input = "  ";
    FILE* in = fmemopen(input, strlen(input), "r");

    struct FPMD_Tokenizer tokenizer;
    fpmd_tokenizer_init(&tokenizer, in);

    int ret = fpmd_tokenizer_next(&tokenizer);
    if(ret != 1) {
        char msg[256];
        sprintf(msg, "expected to read next token, but got error code %d", ret);
        return test_failed(msg);
    }

    if(tokenizer.currentToken.tokenType != INDENTION) {
        return test_failed("expected token type INDENTION");
    }

    return test_passed();
}

int tokenizer_next_token_multiple_indent()
{
    report_test_start("tokenizer_next_token_multiple_indent");

    char *input = "    ";
    FILE* in = fmemopen(input, strlen(input), "r");

    struct FPMD_Tokenizer tokenizer;
    fpmd_tokenizer_init(&tokenizer, in);

    fpmd_tokenizer_next(&tokenizer);

    int ret = fpmd_tokenizer_next(&tokenizer);

    if(ret != 1) {
        char msg[256];
        sprintf(msg, "expected to read next token, but got error code %d", ret);
        return test_failed(msg);
    }

    if(tokenizer.currentToken.tokenType != INDENTION) {
        return test_failed("expected token type INDENTION");
    }

    return test_passed();
}

int tokenizer_next_token_newline()
{
    report_test_start("tokenizer_next_token_newline");

    char *input = "\n";
    FILE* in = fmemopen(input, strlen(input), "r");

    struct FPMD_Tokenizer tokenizer;
    fpmd_tokenizer_init(&tokenizer, in);

    int ret = fpmd_tokenizer_next(&tokenizer);
    if(ret != 1) {
        char msg[256];
        sprintf(msg, "expected to read next token, but got error code %d", ret);
        return test_failed(msg);
    }

    if(tokenizer.currentToken.tokenType != NEWLINE) {
        return test_failed("expected token type NEWLINE");
    }

    return test_passed();
}

int tokenizer_next_token_multiple_newline()
{
    report_test_start("tokenizer_next_token_eof");

    char *input = "\n\n";
    FILE* in = fmemopen(input, strlen(input), "r");

    struct FPMD_Tokenizer tokenizer;
    fpmd_tokenizer_init(&tokenizer, in);

    fpmd_tokenizer_next(&tokenizer);

    int ret = fpmd_tokenizer_next(&tokenizer);
    if(ret != 1) {
        char msg[256];
        sprintf(msg, "expected to read next token, but got error code %d", ret);
        return test_failed(msg);
    }

    if(tokenizer.currentToken.tokenType != NEWLINE) {
        return test_failed("expected token type NEWLINE");
    }

    return test_passed();
}

int tokenizer_block(){
    report_test_start("tokenizer_block");

    char *input =
    "wall exterior\n"
    "  0 0";

    FILE* in = fmemopen(input, strlen(input), "r");

    struct FPMD_Tokenizer tokenizer;
    fpmd_tokenizer_init(&tokenizer, in);

    fpmd_tokenizer_next(&tokenizer);
    int bufferSize = fpmp_token_buffersize(&tokenizer);
    char buffer1[bufferSize];
    fpmd_token_value(&tokenizer, buffer1, bufferSize);

    if(test_string(buffer1, "wall") != TEST_SUCCESS)
        return TEST_FAILURE; 

    fpmd_tokenizer_next(&tokenizer);
    bufferSize = fpmp_token_buffersize(&tokenizer);
    char buffer2[bufferSize];
    fpmd_token_value(&tokenizer, buffer2, bufferSize);

    if(test_string(buffer2, "exterior") != TEST_SUCCESS)
        return TEST_FAILURE;

    fpmd_tokenizer_next(&tokenizer);
    bufferSize = fpmp_token_buffersize(&tokenizer);
    char buffer3[bufferSize];
    fpmd_token_value(&tokenizer, buffer3, bufferSize);

    if(test_string(buffer3, "NEWLINE") != TEST_SUCCESS)
        return TEST_FAILURE;

    fpmd_tokenizer_next(&tokenizer);
    bufferSize = fpmp_token_buffersize(&tokenizer);
    char buffer4[bufferSize];
    fpmd_token_value(&tokenizer, buffer4, bufferSize);

    if(test_string(buffer4, "INDENTION") != TEST_SUCCESS)
        return TEST_FAILURE;

    fpmd_tokenizer_next(&tokenizer);
    bufferSize = fpmp_token_buffersize(&tokenizer);
    char buffer5[bufferSize];
    fpmd_token_value(&tokenizer, buffer5, bufferSize);

    if(test_string(buffer5, "0") != TEST_SUCCESS)
        return TEST_FAILURE;

    fpmd_tokenizer_next(&tokenizer);
    bufferSize = fpmp_token_buffersize(&tokenizer);
    char buffer6[bufferSize];
    fpmd_token_value(&tokenizer, buffer6, bufferSize);

    if(test_string(buffer6, "0") != TEST_SUCCESS)
        return TEST_FAILURE;


    return test_passed();
}

int main() {
    int result = TEST_SUCCESS;

    result |= tokenizer_initialization_test();
    result |= tokenizer_next_token_test();
    result |= tokenizer_next_token_two_texts();
    result |= tokenizer_next_token_two_texts_space();
    result |= tokenizer_next_token_number();
    result |= tokenizer_next_token_a1_is_valid_text();
    result |= tokenizer_next_token_1a_is_valid_text();
    result |= tokenizer_next_token_quotes();
    result |= tokenizer_next_token_indent();
    result |= tokenizer_next_token_multiple_indent();
    result |= tokenizer_next_token_newline();
    result |= tokenizer_next_token_multiple_newline();
    result |= tokenizer_block();

    return result;
}