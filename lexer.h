#ifndef CSCHEME_LEXER_H
#define CSCHEME_LEXER_H

#include "port.h"

enum token_type {
    TOK_IDENTIFIER,
    TOK_NUMBER,
    TOK_BOOLEAN,
    TOK_STRING,
    TOK_LPAREN,
    TOK_RPAREN,
    TOK_DOT,
    TOK_LAST
};

// Max number of characters a token can have including terminating zero
#define MAX_TOKEN_LENGTH 512

struct token {
    enum token_type type;
    char* str;
};

struct lexer;

struct lexer* create_lexer(struct scm_port* port);
struct token* next_token(struct lexer* lexer);
struct token* peek_token(struct lexer* lexer);

/* #define CSCHEME_LEXER_H */
#endif
