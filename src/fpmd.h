/*****************************************************

Module FPBM - FloorPlan MarkDown



*****************************************************/

#include <stdio.h>
#include <stdbool.h>

//############################################################################
//# START OF ERRORS
//############################################################################

const int FPMD_TOKENIZER_ERROR_EXPECTED_WHITESPACE = -100;
const int FPMD_TOKENIZER_ERROR_UNEXPECTED_CHARACTER_AFTER_NEWLINE = -101;
const int FPMD_TOKENIZER_ERROR_UNEXPECTED_CHARACTER_AFTER_TEXT = -102;
const int FPMD_TOKENIZER_ERROR_UNEXPECTED_CHARACTER_AFTER_QUOTED_TEXT = -103;

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
    STATE_EOF,
    STATE_NEWLINE,
    STATE_QUOTED_TEXT_START,
    STATE_INDENTION_IN_PROGRESS,
    STATE_TEXT_IN_PROGRESS,
    STATE_QUOTED_TEXT_IN_PROGRESS,
    STATE_INDENTION_FINISH, 
    STATE_SEARCH_FOR_NEXT_TOKEN,
    STATE_ERROR
};

struct FPMD_Tokenizer{
    FILE* input;
    enum FPMD_Tokenizer_State state;
    enum FPMD_Tokenizer_State previousState;
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
    tokenizer->previousState = STATE_NEWLINE;
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

enum FPMD_Tokenizer_State fpmb_tokenizer_get_next_state(const struct FPMD_Tokenizer* tokenizer, int c, int* error)
{
    switch(tokenizer->state)
    {
        case STATE_NEWLINE:
        case STATE_INDENTION_FINISH:
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
                return STATE_QUOTED_TEXT_START;
            }
            else if(fpmd_tokenizer_is_newline(c))
            {
                return STATE_NEWLINE;
            }
            else
            {
                *error = FPMD_TOKENIZER_ERROR_UNEXPECTED_CHARACTER_AFTER_NEWLINE;
                return STATE_EOF;
            }
            break;
        case STATE_SEARCH_FOR_NEXT_TOKEN:
            if(fpmd_tokenizer_is_space(c))
            {
                return STATE_SEARCH_FOR_NEXT_TOKEN;
            }
            else if(fpmd_tokenizer_is_text(c))
            {
                return STATE_TEXT_IN_PROGRESS;
            }
            else if(fpmd_tokenizer_is_singlequote(c))
            {
                return STATE_QUOTED_TEXT_START;
            }
            else if(fpmd_tokenizer_is_newline(c))
            {
                return STATE_NEWLINE;
            }
            else
            {
                *error = FPMD_TOKENIZER_ERROR_UNEXPECTED_CHARACTER_AFTER_NEWLINE;
                return STATE_EOF;
            }
            break;
        case STATE_INDENTION_IN_PROGRESS:
            if(fpmd_tokenizer_is_space(c))
            {
                return STATE_INDENTION_FINISH;
            }
            else 
            {
                *error = FPMD_TOKENIZER_ERROR_EXPECTED_WHITESPACE;
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
                *error = FPMD_TOKENIZER_ERROR_UNEXPECTED_CHARACTER_AFTER_TEXT;
                return STATE_EOF;
            }
            break;
        case STATE_QUOTED_TEXT_START:
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
                *error = FPMD_TOKENIZER_ERROR_UNEXPECTED_CHARACTER_AFTER_QUOTED_TEXT;
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
                *error = FPMD_TOKENIZER_ERROR_UNEXPECTED_CHARACTER_AFTER_QUOTED_TEXT;
                return STATE_ERROR;
            }
            break;

        default:
            break;
    }

    return STATE_SEARCH_FOR_NEXT_TOKEN;
 
}

void fpmb_tokenizer_move_next_state( struct FPMD_Tokenizer* tokenizer, int c, int* error)
{
    tokenizer->previousState = tokenizer->state;
    tokenizer->state = fpmb_tokenizer_get_next_state(tokenizer, c, error);
}

int fpmd_tokenizer_next(struct FPMD_Tokenizer* tokenizer)
{
    // Assume that this is the start of a new token
    tokenizer->currentToken.start = ftell(tokenizer->input);
    tokenizer->currentToken.length = 0;

    //const int TOKEN_BUFFER_SIZE = 256;

    int c;
    //char buffer[TOKEN_BUFFER_SIZE];
    //int bufferPosition = 0;
    do{
        c = fgetc(tokenizer->input);
        
        int error = 0;
        fpmb_tokenizer_move_next_state(tokenizer, c, &error);

        if(tokenizer->state == STATE_ERROR)
        {
            return error; // TODO return error code
        }

        if(tokenizer->state == STATE_INDENTION_FINISH)
        {
            tokenizer->currentToken.tokenType = INDENTION;
            tokenizer->currentToken.length += 1;
            return true;
        }

        if(tokenizer->state == STATE_QUOTED_TEXT_START)
        {
            // Skip starting quote
            tokenizer->currentToken.start += 1;
        }

        if(tokenizer->state == STATE_NEWLINE)
        {
            tokenizer->currentToken.tokenType = NEWLINE;
            tokenizer->currentToken.length += 1;
            return true;
        }
        else if(tokenizer->state == STATE_INDENTION_IN_PROGRESS)
        {
            tokenizer->currentToken.tokenType = INDENTION;
            tokenizer->currentToken.length += 1;
        }
        else if(tokenizer->state == STATE_TEXT_IN_PROGRESS)
        {
            tokenizer->currentToken.tokenType = TEXT;
            tokenizer->currentToken.length += 1;
        }
        else if(tokenizer->state == STATE_QUOTED_TEXT_IN_PROGRESS)
        {
            tokenizer->currentToken.tokenType = TEXT;
            tokenizer->currentToken.length += 1;
        }
        else if(tokenizer->state == STATE_SEARCH_FOR_NEXT_TOKEN)
        {
            // Finalize token
            if(tokenizer->previousState == STATE_INDENTION_IN_PROGRESS
            || tokenizer->previousState == STATE_TEXT_IN_PROGRESS
            || tokenizer->previousState == STATE_QUOTED_TEXT_IN_PROGRESS)
            {
                return true;
            }
            else{
                // Continue searching for next token
                tokenizer->currentToken.start = ftell(tokenizer->input);
                tokenizer->currentToken.length = 0;
            }
        }


    } while (c != EOF );

    if(tokenizer->previousState == STATE_TEXT_IN_PROGRESS
    || tokenizer->previousState == STATE_INDENTION_IN_PROGRESS)
    {
        return true; // if quoted test not closed, error should be raised
    }

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