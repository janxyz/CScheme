// required for cmocka
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
// strcmp
#include <string.h>
// EOF
#include <stdio.h>

#include "../obj.h"
#include "../string.h"
#include "../pair.h"
#include "../symbol.h"
#include "../procedure.h"
#include "../port.h"

// Mocks
void __wrap_exit_with_error(char const* format, ...)
{
    check_expected(format);
}

static void test_scm_eq_p()
{
    assert_ptr_equal(scm_eq_p(scm_false, scm_false), scm_true);
    assert_ptr_equal(scm_eq_p(scm_true, scm_true), scm_true);
    assert_ptr_equal(scm_eq_p(scm_true, scm_false), scm_false);
}

static void test_scm_eq_p_scm_string()
{
    struct scm_obj* s1 = create_string("Hello World");
    struct scm_obj* s2 = create_string("Hello World");
    assert_ptr_equal(scm_eq_p(s1, s1), scm_true);
    assert_ptr_equal(scm_eq_p(s1, s2), scm_false);
}

static void test_scm_boolean_p()
{
    assert_ptr_equal(scm_boolean_p(scm_true), scm_true);
    assert_ptr_equal(scm_boolean_p(scm_false), scm_true);
}

static void test_scm_string()
{
    struct scm_obj* s = create_string("Hello World");
    assert_ptr_equal(scm_string_p(s), scm_true);
    assert_ptr_equal(scm_string_p(scm_true), scm_false);
    assert_true(strcmp("Hello World", c_string((void*)s)) == 0);
}

static void test_scm_null_p()
{
    assert_ptr_equal(scm_null_p(scm_nil), scm_true);
    assert_ptr_equal(scm_null_p(scm_true), scm_false);
    assert_ptr_equal(scm_null_p(scm_false), scm_false);
}

static void test_scm_cons()
{
    struct scm_obj* car = create_string("CAR");
    struct scm_obj* cdr = create_string("CDR");
    struct scm_obj* p = scm_cons(car, cdr);
    assert_ptr_equal(scm_pair_p(p), scm_true);
    assert_ptr_equal(scm_car(p), car);
    assert_ptr_equal(scm_cdr(p), cdr);
}

static void test_scm_car_error()
{
    expect_string_count(__wrap_exit_with_error, format, "Calling car on non-pair object\n", 3);
    scm_car(scm_nil);
    scm_car(scm_true);
    scm_car(scm_false);
}

static void test_scm_cdr_error()
{
    expect_string_count(__wrap_exit_with_error, format, "Calling cdr on non-pair object\n", 3);
    scm_cdr(scm_nil);
    scm_cdr(scm_true);
    scm_cdr(scm_false);
}

static void test_scm_symbol()
{
    struct scm_obj* s = create_symbol("symbol");
    assert_ptr_equal(scm_symbol_p(s), scm_true);
}

static void test_intern_new()
{
    init_symbol_table();
    struct scm_obj* sym1 = intern("symbol-1");
    struct scm_obj* sym2 = intern("symbol-2");
    assert_ptr_equal(scm_symbol_p(sym1), scm_true);
    assert_ptr_equal(scm_symbol_p(sym2), scm_true);
    assert_ptr_not_equal(sym1, sym2);
}

static void test_intern_existing()
{
    init_symbol_table();
    struct scm_obj* sym1 = intern("symbol-1");
    struct scm_obj* sym2 = intern("symbol-2");
    assert_ptr_equal(intern("symbol-1"), sym1);
    assert_ptr_equal(intern("symbol-2"), sym2);
}

static void test_procedure()
{
    init_symbol_table();
    struct scm_obj* proc = scm_make_procedure(intern("params"), intern("body"), intern("env"));
    assert_ptr_equal(scm_procedure_p(proc), scm_true);
    assert_false(is_primitive(proc));
    assert_ptr_equal(scm_procedure_p(scm_false), scm_false);
    assert_ptr_equal(scm_procedure_parameters(proc), intern("params"));
    assert_ptr_equal(scm_procedure_body(proc), intern("body"));
    assert_ptr_equal(scm_procedure_environment(proc), intern("env"));
}

static void test_procedure_error()
{
    init_symbol_table();
    expect_string_count(__wrap_exit_with_error, format, "Type error: procedure expected\n", 3);
    scm_procedure_parameters(scm_nil);
    scm_procedure_body(intern("body"));
    scm_procedure_environment(scm_true);
}

static struct scm_obj* primitive_fn(struct scm_obj* arguments)
{
    return arguments;
}

static void test_primitive_procedure()
{
    struct scm_obj* proc = make_primitive_procedure(&primitive_fn);
    assert_ptr_equal(scm_procedure_p(proc), scm_true);
    assert_true(is_primitive(proc));
    primitive_function fn = primitive_procedure_function(proc);
    assert_ptr_equal(fn, &primitive_fn);
}

static void test_init_ports()
{
    init_ports();
    struct scm_obj* in_port = scm_current_input_port();
    struct scm_obj* out_port = scm_current_output_port();
    assert_ptr_equal(scm_port_p(in_port), scm_true);
    assert_ptr_equal(scm_port_p(out_port), scm_true);
    assert_ptr_equal(scm_input_port_p(in_port), scm_true);
    assert_ptr_equal(scm_output_port_p(out_port), scm_true);
}

static void test_input_file_port()
{
    init_ports();
    struct scm_obj* p = scm_open_input_file(create_string("tests/text.txt"));
    assert_ptr_equal(scm_input_port_p(p), scm_true);
    assert_int_equal(peek_char((void*)p), 'H');
    assert_int_equal(read_char((void*)p), 'H');
    assert_int_equal(read_char((void*)p), 'e');
    assert_int_equal(read_char((void*)p), 'l');
    assert_int_equal(read_char((void*)p), 'l');
    assert_int_equal(read_char((void*)p), 'o');
    assert_int_equal(read_char((void*)p), '\n');
    assert_int_equal(read_char((void*)p), EOF);
    assert_int_equal(peek_char((void*)p), EOF);
}

static void test_close_input_file_port()
{
    init_ports();
    expect_string(__wrap_exit_with_error, format, "Operation on closed port\n");
    struct scm_obj* p = scm_open_input_file(create_string("tests/text.txt"));
    assert_ptr_equal(scm_port_closed_p(p), scm_false);
    scm_close_port(p);
    assert_ptr_equal(scm_port_closed_p(p), scm_true);
    // Closing multiple times should be possible
    scm_close_port(p);
    read_char((void*)p);
}

static void test_open_input_string_port()
{
    init_ports();
    struct scm_obj* p = scm_open_input_string(create_string("Hi!"));
    assert_ptr_equal(scm_port_p(p), scm_true);
    assert_ptr_equal(scm_input_port_p(p), scm_true);
    assert_ptr_equal(scm_port_closed_p(p), scm_false);
}

static void test_close_input_string_port()
{
    init_ports();
    struct scm_obj* p = scm_open_input_string(create_string("Hi!"));
    struct scm_obj* result = scm_close_port(p);
    assert_ptr_equal(result, p);
    assert_ptr_equal(scm_port_p(p), scm_true);
    assert_ptr_equal(scm_input_port_p(p), scm_true);
    assert_ptr_equal(scm_port_closed_p(p), scm_true);
}

static void test_read_string_port()
{
    init_ports();
    struct scm_obj* p = scm_open_input_string(create_string("Hi!"));
    assert_int_equal(peek_char((void*)p), 'H');
    assert_int_equal(read_char((void*)p), 'H');
    assert_int_equal(read_char((void*)p), 'i');
    assert_int_equal(read_char((void*)p), '!');
    assert_int_equal(read_char((void*)p), EOF);
    assert_int_equal(peek_char((void*)p), EOF);
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
        cmocka_unit_test(test_primitive_procedure),
        cmocka_unit_test(test_init_ports),
        cmocka_unit_test(test_input_file_port),
        cmocka_unit_test(test_close_input_file_port),
        cmocka_unit_test(test_open_input_string_port),
        cmocka_unit_test(test_close_input_string_port),
        cmocka_unit_test(test_read_string_port),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
