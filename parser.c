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

/* ---------- TOKENIZER ---------- */

Token getNextToken() {
    Token token;
    int c;

    while ((c = fgetc(source)) != EOF && isspace(c));

    if (c == EOF) {
        token.type = TOKEN_EOF;
        return token;
    }

    /* Identifier or keyword */
    if (isalpha(c)) {
        int i = 0;
        token.lexeme[i++] = c;

        while (isalnum(c = fgetc(source))) {
            token.lexeme[i++] = c;
        }
        token.lexeme[i] = '\0';
        ungetc(c, source);

        if (strcmp(token.lexeme, "int") == 0)
            token.type = TOKEN_INT;
        else if (strcmp(token.lexeme, "print") == 0)
            token.type = TOKEN_PRINT;
        else
            token.type = TOKEN_ID;

        return token;
    }

    /* Number */
    if (isdigit(c)) {
        int i = 0;
        token.lexeme[i++] = c;

        while (isdigit(c = fgetc(source))) {
            token.lexeme[i++] = c;
        }
        token.lexeme[i] = '\0';
        ungetc(c, source);

        token.type = TOKEN_NUMBER;
        return token;
    }

    /* Symbols */
    token.lexeme[0] = c;
    token.lexeme[1] = '\0';

    switch (c) {
        case '=': token.type = TOKEN_ASSIGN; break;
        case '+': token.type = TOKEN_PLUS; break;
        case ';': token.type = TOKEN_SEMI; break;
        case '(': token.type = TOKEN_LPAREN; break;
        case ')': token.type = TOKEN_RPAREN; break;
        default:  token.type = TOKEN_INVALID;
    }

    return token;
}

/* ---------- PARSER HELPERS ---------- */

void advance() {
    currentToken = getNextToken();
}

void error(const char *msg) {
    printf("Error: %s\n", msg);
    exit(1);
}

/* ---------- PARSER ---------- */

void parseProgram() {
    advance();
    while (currentToken.type != TOKEN_EOF) {
        parseStatement();
    }
}

void parseStatement() {
    if (currentToken.type == TOKEN_INT)
        parseDeclaration();
    else if (currentToken.type == TOKEN_PRINT)
        parsePrint();
    else
        error("Invalid statement");
}

void parseDeclaration() {
    advance(); /* consume int */

    if (currentToken.type != TOKEN_ID)
        error("Expected identifier after int");

    char varName[50];
    strcpy(varName, currentToken.lexeme);
    advance();

    if (currentToken.type != TOKEN_ASSIGN)
        error("Expected =");

    advance();
    int value = parseExpression();

    if (currentToken.type != TOKEN_SEMI)
        error("Missing semicolon");

    addSymbol(varName, value);
    advance();
}

void parsePrint() {
    advance(); /* consume print */

    if (currentToken.type != TOKEN_LPAREN)
        error("Expected (");

    advance();

    if (currentToken.type != TOKEN_ID)
        error("Expected identifier inside print");

    int index = findSymbol(currentToken.lexeme);
    if (index == -1)
        error("Undeclared variable in print");

    printf("%d\n", table[index].value);
    advance();

    if (currentToken.type != TOKEN_RPAREN)
        error("Expected )");

    advance();

    if (currentToken.type != TOKEN_SEMI)
        error("Missing semicolon after print");

    advance();
}

int parseExpression() {
    int left = parseTerm();

    if (currentToken.type == TOKEN_PLUS) {
        advance();
        int right = parseTerm();
        return left + right;
    }

    return left;
}

int parseTerm() {
    int value;

    if (currentToken.type == TOKEN_NUMBER) {
        value = atoi(currentToken.lexeme);
        advance();
        return value;
    }

    if (currentToken.type == TOKEN_ID) {
        int index = findSymbol(currentToken.lexeme);
        if (index == -1)
            error("Undeclared variable");

        value = table[index].value;
        advance();
        return value;
    }

    error("Invalid expression");
    return 0;
}

/* ---------- MAIN ---------- */

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: ./parser <inputfile>\n");
        return 1;
    }

    source = fopen(argv[1], "r");
    if (!source) {
        printf("Cannot open file\n");
        return 1;
    }

    parseProgram();
    fclose(source);
    return 0;
}

"""
gcc parser.c -o parser
./parser input.txt
input.txt
int y = 5;
int x = 20;
int z = x + y;
print(z);
"""