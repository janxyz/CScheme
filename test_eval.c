// required for cmocka
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include "eval.h"
#include "pair.h"
#include "symbol.h"
#include "string.h"

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

int main(void)
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_eval_self_evaluating),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
