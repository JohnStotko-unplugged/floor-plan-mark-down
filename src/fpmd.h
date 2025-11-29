/*****************************************************

Module FPBM - FloorPlan MarkDown



*****************************************************/

#include <stdio.h>
#include <stdbool.h>


enum FPMD_TOKEN_TYPE{
    NONE,
    NEWLINE,
    INDENTION,
    NUMBER,
    TEXT
};

struct FPMD_Token{
    enum FPMD_TOKEN_TYPE tokenType;
    int start ;
    int length ;
};

struct FPMD_Tokenizer{
    FILE* input;
    struct FPMD_Token currentToken;
};

struct FPMD_Token fpmd_token_init()
{
    struct FPMD_Token token;
    token.tokenType = NONE;
    token.start = -1;
    token.length = -1;
    return token;
}

void fpmd_tokenizer_init(struct FPMD_Tokenizer* tokenizer, FILE* input)
{
    tokenizer->input = input;
    tokenizer->currentToken = fpmd_token_init();
}

bool fpmd_tokenizer_next(struct FPMD_Tokenizer* tokenizer)
{
    return false;
}

void fpmd_convert(FILE* input, FILE* output)
{

}