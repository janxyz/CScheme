// required for cmocka
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include "../lexer.h"
#include "../string.h"
#include "../port.h"

// Mocks
void __wrap_exit_with_error(char const* format, ...)
{
    check_expected(format);
}

static void test_lex_empty_string()
{
    init_ports();
    struct scm_obj* string = create_string("");
    struct scm_obj* port = scm_open_input_string(string);
    struct lexer* lexer = create_lexer((void*)port);
    assert_null(peek_token(lexer));
    assert_null(next_token(lexer));
}

static void test_lex_parentheses()
{
    init_ports();
    struct scm_obj* string = create_string("()");
    struct scm_obj* port = scm_open_input_string(string);
    struct lexer* lexer = create_lexer((void*)port);
    struct token* token = next_token(lexer);
    assert_int_equal(token->type, TOK_LPAREN);
    assert_string_equal(token->str, "(");
    token = next_token(lexer);
    assert_int_equal(token->type, TOK_RPAREN);
    assert_string_equal(token->str, ")");
    token = next_token(lexer);
    assert_null(next_token(lexer));
}

static void test_lex_whitespace()
{
    init_ports();
    struct scm_obj* string = create_string(" \n\t ( \n\t ");
    struct scm_obj* port = scm_open_input_string(string);
    struct lexer* lexer = create_lexer((void*)port);
    struct token* token = next_token(lexer);
    assert_int_equal(token->type, TOK_LPAREN);
    assert_string_equal(token->str, "(");
    token = next_token(lexer);
    assert_null(next_token(lexer));
}

static void test_lex_identifier()
{
    init_ports();
    struct scm_obj* string = create_string(
        " a-b :id eq? => a.b.c "
    );
    struct scm_obj* port = scm_open_input_string(string);
    struct lexer* lexer = create_lexer((void*)port);
    struct token* token = next_token(lexer);
    assert_int_equal(token->type, TOK_IDENTIFIER);
    assert_string_equal(token->str, "a-b");
    token = next_token(lexer);
    assert_int_equal(token->type, TOK_IDENTIFIER);
    assert_string_equal(token->str, ":id");
    token = next_token(lexer);
    assert_int_equal(token->type, TOK_IDENTIFIER);
    assert_string_equal(token->str, "eq?");
    token = next_token(lexer);
    assert_int_equal(token->type, TOK_IDENTIFIER);
    assert_string_equal(token->str, "=>");
    token = next_token(lexer);
    assert_int_equal(token->type, TOK_IDENTIFIER);
    assert_string_equal(token->str, "a.b.c");
    token = next_token(lexer);
    assert_null(next_token(lexer));
}

int main(void)
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_lex_empty_string),
        cmocka_unit_test(test_lex_parentheses),
        cmocka_unit_test(test_lex_whitespace),
        cmocka_unit_test(test_lex_identifier),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
