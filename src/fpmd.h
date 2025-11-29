/*****************************************************

Module FPBM - FloorPlan MarkDown



*****************************************************/

#include <stdio.h>
#include <stdbool.h>


typedef enum FPMD_TOKEN_TYPE{
    NEWLINE,
    INDENTION,
    NUMBER,
    TEXT
} FPMD_TOKEN_TYPE;

struct FPMD_Token{
    FPMD_TOKEN_TYPE tokenType;
    int start;
    int length;
};

struct FPMD_Tokenizer{
    FILE* input;
    struct FPMD_Token currentToken;
};

bool fpmd_tokenizer_next(struct FPMD_Tokenizer tokenizer)
{
    return false;
}

void fpmd_convert(FILE* input, FILE* output)
{

}