/*****************************************************

Module FPBM - FloorPlan MarkDown



*****************************************************/

#include <stdio.h>
#include <stdbool.h>


//############################################################################
//# START OF TOKENIZATION
//############################################################################


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

enum FPMD_Tokenizer_State{
    STATE_NEWLINE,
    STATE_INDENTION_IN_PROGRESS,
    STATE_TEXT_IN_PROGRESS,
    STATE_QUOTED_TEXT_IN_PROGRESS,
    STATE_SEARCH_FOR_NEXT_TOKEN,
    STATE_ERROR
};

struct FPMD_Tokenizer{
    FILE* input;
    enum FPMD_Tokenizer_State state;
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
    tokenizer->state = STATE_NEWLINE;
    tokenizer->currentToken = fpmd_token_init();
}

bool fpmd_tokenizer_isvalidtextchar(int c)
{
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

bool fpmd_tokenizer_is_text(int c)
{
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '-' || c == '_';
}

bool fpmd_tokenizer_is_qtext(int c)
{
    return c != '\'' && c != '\n';
}

bool fpmd_tokenizer_is_space(int c)
{
    return c == ' ';
}

bool fpmd_tokenizer_is_singlequote(int c)
{
    return c == '\'';   
}

bool fpmd_tokenizer_is_newline(int c)
{
    return c == '\n';
}

enum FPMD_Tokenizer_State fpmb_tokenizer_get_next_state(const struct FPMD_Tokenizer* tokenizer, int c)
{
    switch(tokenizer->state)
    {
        case STATE_NEWLINE:
            if(fpmd_tokenizer_is_space(c))
            {
                return STATE_INDENTION_IN_PROGRESS;
            }
            else if(fpmd_tokenizer_is_text(c))
            {
                return STATE_TEXT_IN_PROGRESS;
            }
            else if(fpmd_tokenizer_is_singlequote(c))
            {
                return STATE_QUOTED_TEXT_IN_PROGRESS;
            }
            else if(fpmd_tokenizer_is_newline(c))
            {
                return STATE_NEWLINE;
            }
            else
            {
                return STATE_ERROR;
            }
            break;

        case STATE_INDENTION_IN_PROGRESS:
            if(fpmd_tokenizer_is_space(c))
            {
                return STATE_INDENTION_IN_PROGRESS;
            }
            else 
            {
                return STATE_ERROR;
            }
            break;

        case STATE_TEXT_IN_PROGRESS:
            if(fpmd_tokenizer_is_text(c))
            {
                return STATE_TEXT_IN_PROGRESS;
            }
            else if (fpmd_tokenizer_is_space(c) || fpmd_tokenizer_is_newline(c))
            {
                return STATE_SEARCH_FOR_NEXT_TOKEN;
            }
            else{
                return STATE_ERROR;
            }
            break;

        case STATE_QUOTED_TEXT_IN_PROGRESS:
            if(fpmd_tokenizer_is_qtext(c))
            {
                return STATE_QUOTED_TEXT_IN_PROGRESS;
            }
            else if(fpmd_tokenizer_is_singlequote(c))
            {
                return STATE_SEARCH_FOR_NEXT_TOKEN;
            }
            else
            {
                return STATE_ERROR;
            }
            break;

        default:
            break;
    }

    return STATE_SEARCH_FOR_NEXT_TOKEN;
 
}

void fpmb_tokenizer_move_next_state( struct FPMD_Tokenizer* tokenizer, int c)
{
    tokenizer->state = fpmb_tokenizer_get_next_state(tokenizer, c);
}

bool fpmd_tokenizer_next(struct FPMD_Tokenizer* tokenizer)
{
    //const int TOKEN_BUFFER_SIZE = 256;

    int c;
    //char buffer[TOKEN_BUFFER_SIZE];

    do{
        c = fgetc(tokenizer->input);

        fpmb_tokenizer_move_next_state(tokenizer, c);
        


        if(tokenizer->state == STATE_ERROR)
        {
            return false;
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


//############################################################################
//# END OF TOKENIZATION
//############################################################################


void fpmd_convert(FILE* input, FILE* output)
{

}