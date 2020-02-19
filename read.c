#include "read.h"
#include "lexer.h"
#include "symbol.h"
#include "pair.h"
#include "error.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>

static bool parse_expr(struct lexer* lexer, struct scm_obj**);
static bool parse_list(struct lexer* lexer, struct scm_obj** obj);
static bool parse_list_item(struct lexer* lexer, struct scm_obj** obj);

struct scm_obj* scm_read(struct scm_obj* const port)
{
    if (scm_input_port_p(port) == scm_true && scm_port_closed_p(port) == scm_false) {
        struct scm_port* p = (void*)port;
        struct lexer* l = create_lexer(p);
        struct scm_obj* o = NULL;
        parse_expr(l, &o);
        return o;
    }
    exit_with_error("Type error: open input port expected\n");
    return NULL;
}

bool parse_expr(struct lexer* const lexer, struct scm_obj** const obj)
{
    struct token* t = next_token(lexer);
    if (t->type == TOK_LPAREN) {
        return parse_list(lexer, obj);
    } else if (t->type == TOK_IDENTIFIER) {
        *obj = intern(t->str);
        return true;
    }
    exit_with_error("Unknown token\n");
    return false;
}

static bool parse_list(struct lexer* const lexer, struct scm_obj** const obj)
{
    struct token* t = peek_token(lexer);
    if (!t) {
        exit_with_error("Unclosed list\n");
        return false;
    }
    if (t->type == TOK_RPAREN) {
        next_token(lexer);
        *obj = scm_nil;
        return true;
    }
    if (!parse_list_item(lexer, obj)) {
        return false;
    }
    t = next_token(lexer);
    if (!t || t->type != TOK_RPAREN) {
        exit_with_error("Unclosed list\n");
        return false;
    }
    return true;
}

static bool parse_list_item(struct lexer* const lexer, struct scm_obj** const obj)
{
    struct scm_obj* car = NULL;
    struct scm_obj* cdr = NULL;

    if (!parse_expr(lexer, &car)) {
        return false;
    }

    struct token* t = peek_token(lexer);

    /* end of list */
    if (!t || t->type == TOK_RPAREN) {
        *obj = scm_cons(car, scm_nil);
        return true;
    }

    /* dot notation */
    if (t->type == TOK_DOT) {
        /* advance over TOK_DOT */
        next_token(lexer);
        if (!parse_expr(lexer, &cdr)) {
            return false;
        }
        /* We got a cons cell! */
        *obj = scm_cons(car, cdr);
        return true;
    }

    /* Non-Null token that is not TOK_DOT or TOK_RPAREN
       must be another expression */
    if (!parse_list_item(lexer, &cdr)) {
        return false;
    }
    *obj = scm_cons(car, cdr);
    return true;
}
