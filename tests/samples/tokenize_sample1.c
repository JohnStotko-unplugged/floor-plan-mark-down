#include "../../src/fpmd.h"


int main() {
    char* fileName = "samples/example1/example1.md";
    FILE* in = fopen(fileName, "r");

    if(in == NULL) {
        printf("Failed to open file: %s\n", fileName);
        return 1;
    }

    struct FPMD_Tokenizer tokenizer;
    fpmd_tokenizer_init(&tokenizer, in);

    do {
        fpmd_tokenizer_next(&tokenizer);

        //if(ret != 0) {
        //    printf("Error reading token: %d\n", ret);
        //    break;
        //}

        char buffer[FPMD_MAX_TOKEN_LENGTH];

        fpmd_token_value(&tokenizer, buffer, FPMD_MAX_TOKEN_LENGTH);

        printf("Token Type: %d, Value: '%s'\n", tokenizer.currentToken.tokenType, buffer);

    } while(!feof(in));

    return 0;
}