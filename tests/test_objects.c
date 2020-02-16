#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
/* #include <stdint.h> */
#include <cmocka.h>
// strcmp
#include <string.h>

#include "../obj.h"
#include "../string.h"
#include "../pair.h"
#include "../symbol.h"

// Mocks
void __wrap_exit_with_error(char const* format, ...)
{
    check_expected(format);
}

static void test_scm_eq_p(void** state)
{
    (void)state;
    assert_ptr_equal(scm_eq_p(scm_false, scm_false), scm_true);
    assert_ptr_equal(scm_eq_p(scm_true, scm_true), scm_true);
    assert_ptr_equal(scm_eq_p(scm_true, scm_false), scm_false);
}

static void test_scm_eq_p_scm_string(void** state)
{
    (void)state;
    struct scm_obj* s1 = create_string("Hello World");
    struct scm_obj* s2 = create_string("Hello World");
    assert_ptr_equal(scm_eq_p(s1, s1), scm_true);
    assert_ptr_equal(scm_eq_p(s1, s2), scm_false);
}

static void test_scm_boolean_p(void** state)
{
    (void)state;
    assert_ptr_equal(scm_boolean_p(scm_true), scm_true);
    assert_ptr_equal(scm_boolean_p(scm_false), scm_true);
}

static void test_scm_string(void** state)
{
    (void)state;
    struct scm_obj* s = create_string("Hello World");
    assert_ptr_equal(scm_string_p(s), scm_true);
    assert_ptr_equal(scm_string_p(scm_true), scm_false);
    assert_true(strcmp("Hello World", c_string((void*)s)) == 0);
}

static void test_scm_null_p(void** state)
{
    (void)state;
    assert_ptr_equal(scm_null_p(scm_nil), scm_true);
    assert_ptr_equal(scm_null_p(scm_true), scm_false);
    assert_ptr_equal(scm_null_p(scm_false), scm_false);
}

static void test_scm_cons(void** state)
{
    (void)state;
    struct scm_obj* car = create_string("CAR");
    struct scm_obj* cdr = create_string("CDR");
    struct scm_obj* p = scm_cons(car, cdr);
    assert_ptr_equal(scm_pair_p(p), scm_true);
    assert_ptr_equal(scm_car(p), car);
    assert_ptr_equal(scm_cdr(p), cdr);
}

static void test_scm_car_error(void** state)
{
    (void)state;
    expect_any_always(__wrap_exit_with_error, format);
    scm_car(scm_nil);
    scm_car(scm_true);
    scm_car(scm_false);
}

static void test_scm_cdr_error(void** state)
{
    (void)state;
    expect_any_always(__wrap_exit_with_error, format);
    scm_cdr(scm_nil);
    scm_cdr(scm_true);
    scm_cdr(scm_false);
}

static void test_scm_symbol(void** state)
{
    (void)state;
    struct scm_obj* s = create_symbol("symbol");
    assert_ptr_equal(scm_symbol_p(s), scm_true);
}

static void test_intern_new(void** state)
{
    (void)state;
    init_symbol_table();
    struct scm_obj* sym1 = intern("symbol-1");
    struct scm_obj* sym2 = intern("symbol-2");
    assert_ptr_equal(scm_symbol_p(sym1), scm_true);
    assert_ptr_equal(scm_symbol_p(sym2), scm_true);
    assert_ptr_not_equal(sym1, sym2);
}

static void test_intern_existing(void** state)
{
    (void)state;
    init_symbol_table();
    struct scm_obj* sym1 = intern("symbol-1");
    struct scm_obj* sym2 = intern("symbol-2");
    assert_ptr_equal(intern("symbol-1"), sym1);
    assert_ptr_equal(intern("symbol-2"), sym2);
}

int main(void)
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_scm_eq_p),
        cmocka_unit_test(test_scm_eq_p_scm_string),
        cmocka_unit_test(test_scm_boolean_p),
        cmocka_unit_test(test_scm_string),
        cmocka_unit_test(test_scm_null_p),
        cmocka_unit_test(test_scm_cons),
        cmocka_unit_test(test_scm_car_error),
        cmocka_unit_test(test_scm_cdr_error),
        cmocka_unit_test(test_scm_symbol),
        cmocka_unit_test(test_intern_new),
        cmocka_unit_test(test_intern_existing),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
