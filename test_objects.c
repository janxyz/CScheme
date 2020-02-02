#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <cmocka.h>
// strcmp
#include <string.h>

#include "obj.h"
#include "boolean.h"
#include "string.h"

static void test_scm_eq_p(void** state)
{
    (void)state;
    assert_true(scm_eq_p(scm_false, scm_false) == scm_true);
    assert_true(scm_eq_p(scm_true, scm_true) == scm_true);
    assert_true(scm_eq_p(scm_true, scm_false) == scm_false);
}

static void test_scm_eq_p_scm_string(void** state)
{
    (void)state;
    struct scm_obj* s1 = create_string("Hello World");
    struct scm_obj* s2 = create_string("Hello World");
    assert_true(scm_eq_p(s1, s1) == scm_true);
    assert_true(scm_eq_p(s1, s2) == scm_false);
}

static void test_scm_boolean_p(void** state)
{
    (void)state;
    assert_true(scm_boolean_p(scm_true) == scm_true);
    assert_true(scm_boolean_p(scm_false) == scm_true);
}

static void test_scm_string(void** state)
{
    (void)state;
    struct scm_obj* s = create_string("Hello World");
    assert_true(scm_string_p(s) == scm_true);
    assert_true(scm_string_p(scm_true) == scm_false);
    assert_true(strcmp("Hello World", c_string((void*)s)) == 0);
}

int main(void)
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_scm_eq_p),
        cmocka_unit_test(test_scm_eq_p_scm_string),
        cmocka_unit_test(test_scm_boolean_p),
        cmocka_unit_test(test_scm_string),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
