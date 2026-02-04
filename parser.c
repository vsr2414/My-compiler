#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* ---------- TOKEN DEFINITIONS ---------- */

typedef enum {
    TOKEN_INT,
    TOKEN_PRINT,
    TOKEN_ID,
    TOKEN_NUMBER,
    TOKEN_ASSIGN,
    TOKEN_PLUS,
    TOKEN_SEMI,
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_EOF,
    TOKEN_INVALID
} TokenType;

typedef struct {
    TokenType type;
    char lexeme[50];
} Token;

/* ---------- SYMBOL TABLE ---------- */

typedef struct {
    char name[50];
    int value;
} Symbol;

Symbol table[100];
int symbolCount = 0;

/* ---------- GLOBALS ---------- */

FILE *source;
Token currentToken;