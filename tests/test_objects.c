// required for cmocka
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
// strcmp
#include <string.h>

#include "../obj.h"
#include "../string.h"
#include "../pair.h"
#include "../symbol.h"
#include "../procedure.h"

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
    expect_string_count(__wrap_exit_with_error, format, "Calling car on non-pair object\n", 3);
    scm_car(scm_nil);
    scm_car(scm_true);
    scm_car(scm_false);
}

static void test_scm_cdr_error(void** state)
{
    (void)state;
    expect_string_count(__wrap_exit_with_error, format, "Calling cdr on non-pair object\n", 3);
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

static void test_procedure(void** state)
{
    (void)state;
    init_symbol_table();
    struct scm_obj* proc = scm_make_procedure(intern("params"), intern("body"), intern("env"));
    assert_ptr_equal(scm_procedure_p(proc), scm_true);
    assert_ptr_equal(scm_procedure_p(scm_false), scm_false);
    assert_ptr_equal(scm_procedure_parameters(proc), intern("params"));
    assert_ptr_equal(scm_procedure_body(proc), intern("body"));
    assert_ptr_equal(scm_procedure_environment(proc), intern("env"));
}

static void test_procedure_error(void** state)
{
    (void)state;
    init_symbol_table();
    expect_string_count(__wrap_exit_with_error, format, "Type error: procedure expected\n", 3);
    scm_procedure_parameters(scm_nil);
    scm_procedure_body(intern("body"));
    scm_procedure_environment(scm_true);
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
        cmocka_unit_test(test_procedure),
        cmocka_unit_test(test_procedure_error),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
