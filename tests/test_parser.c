// required for cmocka
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include "../read.h"
#include "../string.h"
#include "../port.h"
#include "../obj.h"
#include "../pair.h"
#include "../symbol.h"

// Mocks
void __wrap_exit_with_error(char const* format, ...)
{
    check_expected(format);
}

static void test_read_symbol()
{
    init_symbol_table();
    init_ports();
    struct scm_obj* string = create_string("sym");
    struct scm_obj* port = scm_open_input_string(string);
    struct scm_obj* exp = scm_read(port);
    assert_int_equal(exp->type, TYPE_SYMBOL);
}

static void test_read_empty_list()
{
    init_symbol_table();
    init_ports();
    struct scm_obj* string = create_string("()");
    struct scm_obj* port = scm_open_input_string(string);
    struct scm_obj* exp = scm_read(port);
    assert_ptr_equal(exp, scm_nil);
}

static void test_read_quoted_empty_list()
{
    init_symbol_table();
    init_ports();
    struct scm_obj* string = create_string("(quote ())");
    struct scm_obj* port = scm_open_input_string(string);
    struct scm_obj* exp = scm_read(port);
    assert_ptr_equal(scm_car(exp), intern("quote"));
    assert_ptr_equal(scm_car(scm_cdr(exp)), scm_nil);
}

static void test_read_symbol_list()
{
    init_symbol_table();
    init_ports();
    struct scm_obj* string = create_string("(a b c)");
    struct scm_obj* port = scm_open_input_string(string);
    struct scm_obj* exp = scm_read(port);
    assert_ptr_equal(scm_car(exp), intern("a"));
    assert_ptr_equal(scm_car(scm_cdr(exp)), intern("b"));
    assert_ptr_equal(scm_car(scm_cdr(scm_cdr(exp))), intern("c"));
}

static void test_read_boolean()
{
    init_symbol_table();
    init_ports();
    struct scm_obj* string = create_string("#t#f");
    struct scm_obj* port = scm_open_input_string(string);
    struct scm_obj* exp = scm_read(port);
    assert_ptr_equal(exp, scm_true);
    exp = scm_read(port);
    assert_ptr_equal(exp, scm_false);
}

static void test_read_string()
{
    init_symbol_table();
    init_ports();
    struct scm_obj* string = create_string("\"one\"");
    struct scm_obj* port = scm_open_input_string(string);
    struct scm_obj* exp = scm_read(port);
    assert_int_equal(exp->type, TYPE_STRING);
}

static void test_read_number()
{
    init_symbol_table();
    init_ports();
    struct scm_obj* string = create_string("123");
    struct scm_obj* port  = scm_open_input_string(string);
    struct scm_obj* exp = scm_read(port);
    assert_int_equal(exp->type, TYPE_NUMBER);
}

int main(void)
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_read_symbol),
        cmocka_unit_test(test_read_empty_list),
        cmocka_unit_test(test_read_quoted_empty_list),
        cmocka_unit_test(test_read_symbol_list),
        cmocka_unit_test(test_read_boolean),
        cmocka_unit_test(test_read_string),
        cmocka_unit_test(test_read_number),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
