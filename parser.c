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

/* ---------- FUNCTION DECLARATIONS ---------- */

Token getNextToken();
void advance();
void error(const char *msg);

void parseProgram();
void parseStatement();
void parseDeclaration();
void parsePrint();
int parseExpression();
int parseTerm();

int findSymbol(const char *name);
void addSymbol(const char *name, int value);

/* ---------- SYMBOL TABLE FUNCTIONS ---------- */

int findSymbol(const char *name) {
    for (int i = 0; i < symbolCount; i++) {
        if (strcmp(table[i].name, name) == 0)
            return i;
    }
    return -1;
}

void addSymbol(const char *name, int value) {
    if (findSymbol(name) != -1)
        error("Variable already declared");

    strcpy(table[symbolCount].name, name);
    table[symbolCount].value = value;
    symbolCount++;
}