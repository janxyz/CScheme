// required for cmocka
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include "../eval.h"
#include "../pair.h"
#include "../symbol.h"
#include "../string.h"
#include "../procedure.h"

// Mocks
void __wrap_exit_with_error(char const* format, ...)
{
    check_expected(format);
}

static void test_eval_self_evaluating()
{
    struct scm_obj* const env = scm_nil;
    struct scm_obj* s = create_string("string");
    assert_ptr_equal(scm_eval(scm_true, env), scm_true);
    assert_ptr_equal(scm_eval(scm_false, env), scm_false);
    assert_ptr_equal(scm_eval(s, env), s);
}

static void test_eval_variable_lookup()
{
    init_symbol_table();
    struct scm_obj* var1 = intern("var-1");
    struct scm_obj* val1 = intern("val-1");
    struct scm_obj* var2 = intern("var-2");
    struct scm_obj* val2 = intern("val-2");
    struct scm_obj* frame1 = scm_cons(scm_cons(var1, val1), scm_nil);
    struct scm_obj* frame2 = scm_cons(scm_cons(var2, val2), scm_nil);
    // Shadowing old var1 value
    struct scm_obj* frame3 = scm_cons(scm_cons(var1, val2), scm_nil);
    struct scm_obj* env = scm_cons(frame2, scm_cons(frame1, scm_nil));
    struct scm_obj* env2 = scm_cons(frame3, env);
    assert_ptr_equal(scm_eval(var1, env), val1);
    assert_ptr_equal(scm_eval(var2, env), val2);
    assert_ptr_equal(scm_eval(var1, env2), val2);
}

static void test_eval_quote()
{
    init_symbol_table();
    struct scm_obj* const env = scm_nil;
    struct scm_obj* exp = scm_cons(intern("quote"), scm_cons(intern("sym"), scm_nil));
    assert_ptr_equal(scm_eval(exp, env), intern("sym"));
}

static void test_eval_quoted_empty_list()
{
    init_symbol_table();
    struct scm_obj* env = scm_nil;
    struct scm_obj* exp = scm_cons(scm_nil, scm_nil);
    exp = scm_cons(intern("quote"), exp);
    struct scm_obj* result = scm_eval(exp, env);
    assert_ptr_equal(result, scm_nil);
}

static void test_eval_if()
{
    init_symbol_table();
    struct scm_obj* env = scm_nil;
    struct scm_obj* true_var = intern("true");
    struct scm_obj* true_val = intern("yes");
    struct scm_obj* false_var = intern("false");
    struct scm_obj* false_val = intern("no");
    struct scm_obj* frame = scm_cons(scm_cons(true_var, true_val), scm_nil);
    frame = scm_cons(scm_cons(false_var, false_val), frame);
    env = scm_cons(frame, env);
    struct scm_obj* if_cddr = scm_cons(true_var, scm_cons(false_var, scm_nil));
    struct scm_obj* if_sym = intern("if");

    struct scm_obj* exp = scm_cons(if_sym, scm_cons(scm_true, if_cddr));
    assert_ptr_equal(scm_eval(exp, env), true_val);

    exp = scm_cons(if_sym, scm_cons(scm_false, if_cddr));
    assert_ptr_equal(scm_eval(exp, env), false_val);
}

static void test_eval_lambda()
{
    init_symbol_table();
    struct scm_obj* env = scm_nil;
    // (lambda () #t)
    struct scm_obj* lambda_exp = scm_cons(scm_true, scm_nil);
    lambda_exp = scm_cons(scm_nil, lambda_exp);
    lambda_exp = scm_cons(intern("lambda"), lambda_exp);

    struct scm_obj* proc = scm_eval(lambda_exp, env);
    assert_ptr_equal(scm_procedure_p(proc), scm_true);
    assert_ptr_equal(scm_procedure_parameters(proc), scm_nil);
    assert_ptr_equal(scm_car(scm_procedure_body(proc)), scm_true);
    assert_ptr_equal(scm_procedure_environment(proc), env);
}

static void test_eval_list()
{
    init_symbol_table();
    struct scm_obj* env = scm_nil;
    struct scm_obj* quote_exp = scm_cons(intern("quote"), scm_cons(intern("foo"), scm_nil));
    struct scm_obj* list = scm_nil;
    struct scm_obj* str = create_string("str");
    list = scm_cons(quote_exp, scm_cons(str, list));
    // ((quote foo) "str") => (foo "str")
    struct scm_obj* result = eval_list(list, env);
    assert_ptr_equal(scm_car(result), intern("foo"));
    assert_ptr_equal(scm_car(scm_cdr(result)), str);
}

struct scm_obj* primitive_fn(struct scm_obj* args)
{
    return scm_car(args);
}

static void test_eval_primitive_procedure()
{
    init_symbol_table();
    struct scm_obj* proc = make_primitive_procedure(&primitive_fn);
    struct scm_obj* frame = scm_cons(scm_cons(intern("fn"), proc), scm_nil);
    struct scm_obj* env = scm_cons(frame, scm_nil);
    struct scm_obj* arg = scm_true;
    struct scm_obj* result = scm_eval(scm_cons(intern("fn"), scm_cons(arg, scm_nil)), env);
    assert_ptr_equal(result, arg);
}

static void test_eval_sequence()
{
    init_symbol_table();
    struct scm_obj* env = scm_nil;
    struct scm_obj* quote_exp = scm_cons(intern("quote"), scm_cons(intern("foo"), scm_nil));
    struct scm_obj* list = scm_nil;
    struct scm_obj* str = create_string("str");
    list = scm_cons(str, scm_cons(quote_exp, list));
    // ("str" (quote foo)) => foo
    struct scm_obj* result = eval_sequence(list, env);
    assert_ptr_equal(result, intern("foo"));
}

static void test_eval_compound_procedure()
{
    init_symbol_table();
    // test the identity function
    struct scm_obj* parameters = scm_cons(intern("x"), scm_nil);
    struct scm_obj* body = scm_cons(intern("x"), scm_nil);
    struct scm_obj* proc = scm_make_procedure(parameters, body, scm_nil);
    struct scm_obj* frame = scm_cons(scm_cons(intern("id"), proc), scm_nil);
    struct scm_obj* env = scm_cons(frame, scm_nil);
    struct scm_obj* arg = create_string("str");
    struct scm_obj* application = scm_cons(intern("id"), scm_cons(arg, scm_nil));
    struct scm_obj* result = scm_eval(application, env);
    assert_ptr_equal(result, arg);
}

int main(void)
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_eval_self_evaluating),
        cmocka_unit_test(test_eval_variable_lookup),
        cmocka_unit_test(test_eval_quote),
        cmocka_unit_test(test_eval_quoted_empty_list),
        cmocka_unit_test(test_eval_if),
        cmocka_unit_test(test_eval_lambda),
        cmocka_unit_test(test_eval_list),
        cmocka_unit_test(test_eval_primitive_procedure),
        cmocka_unit_test(test_eval_sequence),
        cmocka_unit_test(test_eval_compound_procedure),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
