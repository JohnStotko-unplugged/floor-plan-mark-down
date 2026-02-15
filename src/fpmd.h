/*****************************************************

Module FPBM - FloorPlan MarkDown



*****************************************************/

#include <stdio.h>
#include <stdbool.h>
#include <stdarg.h>


//#define ENABLE_LOGGING

void log_msg(const char *fmt, ...)
{
#ifdef ENABLE_LOGGING
    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);   // the v* family takes a va_list
    va_end(args);
#endif
}



//############################################################################
//# START OF ERRORS
//############################################################################

const int FPMD_TOKENIZER_ERROR_EXPECTED_WHITESPACE = -100;
const int FPMD_TOKENIZER_ERROR_UNEXPECTED_CHARACTER_AFTER_NEWLINE = -101;
const int FPMD_TOKENIZER_ERROR_UNEXPECTED_CHARACTER_AFTER_TEXT = -102;
const int FPMD_TOKENIZER_ERROR_UNEXPECTED_CHARACTER_AFTER_QUOTED_TEXT = -103;

//############################################################################
//# START OF CONSTANTS
//############################################################################

const int FPMD_MAX_TOKEN_LENGTH = 64;

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
    char token[64];
    int length ;
};

enum FPMD_Tokenizer_State{
    STATE_EOF,
    STATE_NEWLINE,
    STATE_INDENTION_IN_PROGRESS,
    STATE_INDENTION_FINISH, 
    STATE_TEXT_IN_PROGRESS,
    STATE_QUOTED_TEXT_START,
    STATE_QUOTED_TEXT_IN_PROGRESS,
    STATE_SEARCH_FOR_NEXT_TOKEN,
    STATE_ERROR
};

struct FPMD_Tokenizer{
    FILE* input;
    enum FPMD_Tokenizer_State previousState;
    struct FPMD_Token currentToken;
};

struct FPMD_Token fpmd_token_init()
{
    struct FPMD_Token token;
    token.tokenType = NONE;
    token.length = 0;

    return token;
}

void fpmd_token_reset(struct FPMD_Token* token)
{
    for(int i = 0; i < FPMD_MAX_TOKEN_LENGTH; i++)
    {
        token->token[i] = '\0';
    }

    token->length = 0;
    token->tokenType = NONE;
}

void fpmd_token_append(struct FPMD_Token* token, char c)
{
    token->token[token->length] = c;
    token->length++;
}

void fpmd_tokenizer_init(struct FPMD_Tokenizer* tokenizer, FILE* input)
{
    tokenizer->input = input;
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

enum FPMD_Tokenizer_State fpmb_tokenizer_get_next_state_newline(int c, int* error)
{
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
}

enum FPMD_Tokenizer_State fpmb_tokenizer_get_next_state_search(int c, int* error)
{
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
}

enum FPMD_Tokenizer_State fpmb_tokenizer_get_next_state_indention_ip(int c, int* error)
{
    if(fpmd_tokenizer_is_space(c))
    {
        return STATE_INDENTION_FINISH;
    }
    else 
    {
        *error = FPMD_TOKENIZER_ERROR_EXPECTED_WHITESPACE;
        return STATE_ERROR;
    }
}

enum FPMD_Tokenizer_State fpmb_tokenizer_get_next_state_text_ip(int c, int* error)
{
    if(fpmd_tokenizer_is_newline(c))
    {
        return STATE_NEWLINE;
    }
    else if(fpmd_tokenizer_is_text(c))
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
}

enum FPMD_Tokenizer_State fpmb_tokenizer_get_next_state_qtext_start(int c, int* error)
{
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
}

enum FPMD_Tokenizer_State fpmb_tokenizer_get_next_state_qtext_ip(int c, int* error)
{
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
}

enum FPMD_Tokenizer_State fpmb_tokenizer_get_next_state(const enum FPMD_Tokenizer_State previousState, int c, int* error)
{
    switch(previousState)
    {
        case STATE_NEWLINE:
        case STATE_INDENTION_FINISH:
            return fpmb_tokenizer_get_next_state_newline(c, error);
        case STATE_SEARCH_FOR_NEXT_TOKEN:
            return fpmb_tokenizer_get_next_state_search(c, error);
        case STATE_INDENTION_IN_PROGRESS:
            return fpmb_tokenizer_get_next_state_indention_ip(c, error);
        case STATE_TEXT_IN_PROGRESS:
            return fpmb_tokenizer_get_next_state_text_ip(c, error);
        case STATE_QUOTED_TEXT_START:
            return fpmb_tokenizer_get_next_state_qtext_start(c, error);
        case STATE_QUOTED_TEXT_IN_PROGRESS:
            return fpmb_tokenizer_get_next_state_qtext_ip(c, error);
        case STATE_EOF:
            return STATE_EOF;
        default:
            return STATE_ERROR;
    }
}

int fpmd_tokenizer_next(struct FPMD_Tokenizer* tokenizer)
{
    /*
    
    Current flaw in logic... consider
    
    abc\n
    
    after reading 'c', the tokenizer moves to the NEWLINE state
    without finishing the abc token.

    The tokenizer always reads the next character when moving next, 
    so after reading '\n', it would need to  

    1) Finish the current token `abc`
    2) Remember it was still on `\n` when next is called again

    For this, I think the tokenizer needs to remember 2 characters
    - the previous character, and the current one.

    */

    struct FPMD_Token* currentToken = &(tokenizer->currentToken);
    log_msg("reset");
    fpmd_token_reset(currentToken);
    log_msg("\n");
    int c;

    do{
        int error = 0;
        int advance = 1; // if 0, then unget from stream
        int append = 0; // if 0, then do not append to token
        int finish = 0; // if 0, then continue collecting token

        c = fgetc(tokenizer->input);

        enum FPMD_Tokenizer_State nextState = fpmb_tokenizer_get_next_state(tokenizer->previousState, c, &error);
        switch (nextState)
        {
        case STATE_ERROR:
            return error;
        case STATE_INDENTION_FINISH:
            currentToken->tokenType = INDENTION;
            append = 1;
            finish = 1; 
            break;    
        case STATE_QUOTED_TEXT_START:
            currentToken->tokenType = TEXT;
            break;
        case STATE_NEWLINE:

            if(tokenizer->previousState == STATE_INDENTION_IN_PROGRESS
            || tokenizer->previousState == STATE_TEXT_IN_PROGRESS
            || tokenizer->previousState == STATE_QUOTED_TEXT_IN_PROGRESS)
            {
                advance = 0;
                finish = 1;
            }
            else
            {
                tokenizer->currentToken.tokenType = NEWLINE;
                append = 1;
                finish = 1;
            }
            break;
        case STATE_INDENTION_IN_PROGRESS:
            tokenizer->currentToken.tokenType = INDENTION;
            append = 1;
            break;
        case STATE_TEXT_IN_PROGRESS:
            currentToken->tokenType = TEXT;
            append = 1;
            break;
        case STATE_QUOTED_TEXT_IN_PROGRESS:
            currentToken->tokenType = TEXT;
            append = 1;
            break;
        case STATE_SEARCH_FOR_NEXT_TOKEN:
            if(tokenizer->previousState == STATE_INDENTION_IN_PROGRESS
            || tokenizer->previousState == STATE_TEXT_IN_PROGRESS
            || tokenizer->previousState == STATE_QUOTED_TEXT_IN_PROGRESS)
            {
                finish = 1;
            }
            break;
        case STATE_EOF:
            if(tokenizer->previousState == STATE_TEXT_IN_PROGRESS
            || tokenizer->previousState == STATE_INDENTION_IN_PROGRESS)
            {
                finish = 1;
                // if quoted test not closed, error should be raised
            }
        default:
            break;
        }

        log_msg("Char: %c NS: %d PS: %d ap: %d ad: %d fi: %d TL: %d\n", c, nextState, tokenizer->previousState, append, advance, finish, tokenizer->currentToken.length);

        if(append)
        {
            fpmd_token_append(currentToken, c);
        }

        if(!advance)
        {
            ungetc(c, tokenizer->input);
        }

        if(finish > 0 || advance > 0)
        {
            tokenizer->previousState = nextState;
        }

        log_msg("Char: %c NS: %d PS: %d ap: %d ad: %d fi: %d TL: %d\n", c, nextState, tokenizer->previousState, append, advance, finish, tokenizer->currentToken.length);

        if(finish)
        {
            log_msg("Finish\n");
            return true;
        }


    } while (c != EOF );

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
    if(tokenizer->currentToken.tokenType == NONE)
    {
        return TOKEN_VALUE_ERROR_INVALID_TOKEN_TYPE;
    }

    if(buffer_size < fpmp_token_buffersize(tokenizer)) {
        return TOKEN_VALUE_ERROR_BUFFER_TOO_SMALL;
    }

    memcpy(buffer, tokenizer->currentToken.token, tokenizer->currentToken.length + 1);

    return TOKEN_VALUE_SUCCESS;
}


//############################################################################
//# END OF TOKENIZATION
//############################################################################

//############################################################################
//# BEGIN OF Parser
//############################################################################



struct FPMD_Parser{
  struct FPMD_Tokenizer tokenizer;
};

// Rather than constructing the tree in memory, we will provide functions to traverse the tree directly from the file.

// Get Next Item
// gets the next item in the syntax tree. 
// If return value is 0, then no item was retrieved, indicating end of items or an error.
// if error is 0, no error occurred.
// If an item is retrieved, it becomes the current item for subsequent calls to Get Next Row and Get Next Data.
// Calling Get Next Item while reading rows or data will skip to the next item.

// Get Next Row
// progress to the next row in the parent item. 
// If return value is 0, then no data was retrieved, indicating end of row or an error.
// if error is 0, no error occurred.
// If an row is retrieved, it becomes the current row for subsequent calls to Get Next Data.
// Calling Get Next Row while reading data will skip to the next row.

// Get Next Data
// gets the next data entry for the current row. 
// If return value is 0, then no data was retrieved, indicating end of row or an error.
// if error is 0, no error occurred.

//############################################################################
//# END OF Parser
//############################################################################



enum FPMD_ACTION {
    FPMD_ACTION_DEFAULT,
    FPMD_ACTION_TOKENIZE
};

void fpmd_convert(FILE* input, FILE* output, enum FPMD_ACTION action)
{
    struct FPMD_Tokenizer tokenizer;
    fpmd_tokenizer_init(&tokenizer, input);

    do {
        fpmd_tokenizer_next(&tokenizer);

        if(action == FPMD_ACTION_TOKENIZE)
        {
            if(tokenizer.currentToken.tokenType == NEWLINE)
            {
                fprintf(output, "\n");
            }
            else if(tokenizer.currentToken.tokenType == INDENTION)
            {
                fprintf(output, "`->`  ");
            }
            else
            {
                fprintf(output, "`%s`  ", tokenizer.currentToken.token);
            }
        }

    } while(!feof(input));
}