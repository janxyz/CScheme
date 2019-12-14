#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <cmocka.h>

#include "obj.h"
#include "boolean.h"

static void test_scm_eq_p(void** state)
{
    (void)state;
    assert_true(scm_eq_p(scm_false, scm_false) == scm_true);
    assert_true(scm_eq_p(scm_true, scm_true) == scm_true);
    assert_true(scm_eq_p(scm_true, scm_false) == scm_false);
}

static void test_scm_boolean_p(void** state)
{
    (void)state;
    assert_true(scm_boolean_p(scm_true) == scm_true);
    assert_true(scm_boolean_p(scm_false) == scm_true);
}

int main(void)
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_scm_eq_p),
        cmocka_unit_test(test_scm_boolean_p),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
