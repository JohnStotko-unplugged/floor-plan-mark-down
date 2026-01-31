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

        int bufferSize = tokenizer.currentToken.length + 1;
        char buffer[bufferSize];

        fpmd_token_value(&tokenizer, buffer, bufferSize);

        printf("Token Type: %d, Value: '%s'\n", tokenizer.currentToken.tokenType, buffer);

    } while(!feof(in));

    return 0;
}