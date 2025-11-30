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

bool fpmd_tokenizer_isvalidtextchar(int c)
{
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

bool fpmd_tokenizer_next(struct FPMD_Tokenizer* tokenizer)
{
    int c;

    do{
        c = fgetc(tokenizer->input);

        if(fpmd_tokenizer_isvalidtextchar(c))
        {
            tokenizer->currentToken.tokenType = TEXT;
            tokenizer->currentToken.start = ftell(tokenizer->input) - 1;
            tokenizer->currentToken.length = 1;

            while(true)
            {
                c = fgetc(tokenizer->input);
                if(fpmd_tokenizer_isvalidtextchar(c))
                {
                    tokenizer->currentToken.length += 1;
                }
                else
                {
                    ungetc(c, tokenizer->input);
                    break;
                }
            }

            return true;
        }

    } while (c != EOF);

    return false;
}

void fpmd_convert(FILE* input, FILE* output)
{

}