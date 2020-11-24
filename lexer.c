#include "lexer.h"
#include "error.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>

static int lexer_getc(struct lexer* l);
static int lexer_getc_no_buffer(struct lexer* l);
static int lexer_peek(struct lexer* l);
static struct token* lex_token(struct lexer* l);
static struct token* lex_whitespace(struct lexer* l);
static struct token* lex_identifier(struct lexer* l);
static struct token* lex_number_sign(struct lexer* l);
static struct token* lex_string(struct lexer* const l);
static struct token* accept_token(struct lexer* l, enum token_type type);
static bool is_delimiter(int c);
static bool is_identifier_initial(int c);
static bool is_identifier_subsequent(int c);
static bool is_string_delimiter(int c);

struct lexer {
    struct scm_port* port;
    char token_buffer[MAX_TOKEN_LENGTH];
    size_t buffer_pos;
    /* keep this around so the parser doesn't have to */
    struct token* peeked_token;
};

struct lexer* create_lexer(struct scm_port* const port)
{
    struct lexer* l = malloc(sizeof(*l));
    l->port = port;
    l->buffer_pos = 0;
    l->peeked_token = NULL;
    return l;
}

struct token* next_token(struct lexer* const l)
{
    if (l->peeked_token) {
        struct token* t = l->peeked_token;
        l->peeked_token = NULL;
        return t;
    } else {
        return lex_token(l);
    }
}

struct token* peek_token(struct lexer* const l)
{
    if (l->peeked_token) {
        return l->peeked_token;
    } else {
        l->peeked_token = lex_token(l);
        return l->peeked_token;
    }
}

static int lexer_getc(struct lexer* const l)
{
    // Make sure that there is still 1 character for the terminating zero
    if (l->buffer_pos >= MAX_TOKEN_LENGTH - 1) {
        exit_with_error("Token too long\n");
    }
    int c = read_char(l->port);
    l->token_buffer[l->buffer_pos] = c;
    l->buffer_pos += 1;
    return c;
}

static int lexer_getc_no_buffer(struct lexer* const l)
{
    return read_char(l->port);
}

static int lexer_peek(struct lexer* const l)
{
    return peek_char(l->port);
}

static struct token* lex_token(struct lexer* const l)
{
    l->buffer_pos = 0;
    int c = lexer_getc(l);

    if (isspace(c)) {
        lex_whitespace(l);
        return lex_token(l);
    } else if (c == '(') {
        return accept_token(l, TOK_LPAREN);
    } else if (c == ')') {
        return accept_token(l, TOK_RPAREN);
    } else if (c == '#') {
        return lex_number_sign(l);
    } else if (is_identifier_initial(c)) {
        return lex_identifier(l);
    } else if (is_string_delimiter(c)) {
        // Do not include string delimiter in token string
        l->buffer_pos = 0;
        return lex_string(l);
    } else if (c == EOF) {
        return NULL;
    } else {
        exit_with_error("Unrecognized char at line %zu column %zu: %c\n", 0l, 0l, c);
    }
    return NULL;
}

static struct token* lex_whitespace(struct lexer* const l)
{
    while (isspace(lexer_peek(l))) {
        lexer_getc_no_buffer(l);
    }
    return NULL;
}

static struct token* lex_identifier(struct lexer* const l)
{
    while (is_identifier_subsequent(lexer_peek(l))) {
        lexer_getc(l);
    }
    if (is_delimiter(lexer_peek(l))) {
        return accept_token(l, TOK_IDENTIFIER);
    }
    exit_with_error("Identifier not followed by delimiter\n");
    return NULL;
}

static struct token* lex_number_sign(struct lexer* l)
{
    int c = lexer_getc(l);
    if (c == 't' || c == 'f') {
        return accept_token(l, TOK_BOOLEAN);
    }
    exit_with_error("Unknown token\n");
    return NULL;
}

static struct token* lex_string(struct lexer* const l)
{
    int c;
    int prev_c = - 1;
    while (true) {
        c = lexer_getc(l);
        if (c == EOF) {
            exit_with_error("String without closing delimiter\n");
            return NULL;
        } else if (is_string_delimiter(c)) {
            // Check if delimiter was escaped
            if (prev_c == '\\') {
                // Overwrite escape character with delimiter character
                l->buffer_pos -= 1;
                l->token_buffer[l->buffer_pos - 1] = '"';
            } else {
                // Do not include string delimiter in token string
                l->buffer_pos -= 1;
                return accept_token(l, TOK_STRING);
            }
        }
        prev_c = c;
    }
}

static struct token* accept_token(struct lexer* const l, enum token_type type)
{
    struct token* t = malloc(sizeof(*t));
    t->type = type;
    l->token_buffer[l->buffer_pos] = '\0';
    size_t len = l->buffer_pos + 1;
    t->str = malloc(len);
    memcpy(t->str, l->token_buffer, len);
    return t;
}

static bool is_delimiter(int c)
{
    return isspace(c) || c == '(' || c == ')' || c == ';' || c == '\0' || c == EOF;
}

static bool is_identifier_initial(int c)
{
    return isalpha(c) || (c != '\0' && strchr("!$%&*/:<=>?~_^", c));
}

static bool is_identifier_subsequent(int c)
{
    return is_identifier_initial(c) || isdigit(c) ||
        c == '.' || c == '+' || c == '-' || c == '@';
}

static bool is_string_delimiter(int c)
{
    return c == '"';
}
