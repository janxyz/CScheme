// required for cmocka
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include "../eval.h"
#include "../pair.h"
#include "../symbol.h"
#include "../string.h"

// Mocks
void __wrap_exit_with_error(char const* format, ...)
{
    check_expected(format);
}

static void test_eval_self_evaluating(void** state)
{
    (void)state;
    struct scm_obj const* const env = scm_nil;
    struct scm_obj* s = create_string("string");
    assert_true(eval((void*)scm_true, env) == scm_true);
    assert_true(eval((void*)scm_false, env) == scm_false);
    assert_true(eval((void*)scm_nil, env) == scm_nil);
    assert_true(eval(s, env) == s);
}

static void test_eval_variable_lookup(void** state)
{
    (void)state;
    init_symbol_table();
    struct scm_obj* var1 = intern("var-1");
    struct scm_obj* val1 = intern("val-1");
    struct scm_obj* var2 = intern("var-2");
    struct scm_obj* val2 = intern("val-2");
    struct scm_obj* frame1 = scm_cons(scm_cons(var1, val1), (void*)scm_nil);
    struct scm_obj* frame2 = scm_cons(scm_cons(var2, val2), (void*)scm_nil);
    // Shadowing old var1 value
    struct scm_obj* frame3 = scm_cons(scm_cons(var1, val2), (void*)scm_nil);
    struct scm_obj* env = scm_cons(frame2, scm_cons(frame1, (void*)scm_nil));
    struct scm_obj* env2 = scm_cons(frame3, env);
    assert_true(eval(var1, env) == val1);
    assert_true(eval(var2, env) == val2);
    assert_true(eval(var1, env2) == val2);
}
#include <stdio.h>
static void test_eval_quote(void** state)
{
    (void)state;
    init_symbol_table();
    struct scm_obj const* const env = scm_nil;
    struct scm_obj* exp = scm_cons(intern("quote"), scm_cons(intern("sym"), (void*)scm_nil));
    assert_ptr_equal(eval(exp, env), intern("sym"));
}

int main(void)
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_eval_self_evaluating),
        cmocka_unit_test(test_eval_variable_lookup),
        cmocka_unit_test(test_eval_quote),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
