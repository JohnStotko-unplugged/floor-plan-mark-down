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

const int TOKEN_VALUE_ERROR_BUFFER_TOO_SMALL = -1;
const int TOKEN_VALUE_ERROR_INVALID_TOKEN_TYPE = -2;
const int TOKEN_VALUE_SUCCESS = 0;

int fpmp_token_buffersize(struct FPMD_Tokenizer* tokenizer)
{
    return tokenizer->currentToken.length + 1;
}

int fpmd_token_value(struct FPMD_Tokenizer* tokenizer, char buffer[], int buffer_size)
{
    if(tokenizer->currentToken.tokenType != TEXT && tokenizer->currentToken.tokenType != NUMBER) {
        return TOKEN_VALUE_ERROR_INVALID_TOKEN_TYPE;
    }

    if(buffer_size < fpmp_token_buffersize(tokenizer)) {
        return TOKEN_VALUE_ERROR_BUFFER_TOO_SMALL;
    }

    fseek(tokenizer->input, tokenizer->currentToken.start, SEEK_SET);
    fread(buffer, sizeof(char), tokenizer->currentToken.length, tokenizer->input);
    buffer[tokenizer->currentToken.length] = '\0';

    return TOKEN_VALUE_SUCCESS;
}

void fpmd_convert(FILE* input, FILE* output)
{

}