// required for cmocka
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdint.h>

#include "../obj.h"
#include "../pair.h"
#include "../number.h"

// Mocks
void __wrap_exit_with_error(char const* format, ...)
{
    check_expected(format);
}

static void test_scm_number()
{
    struct scm_obj* n = create_number(123);
    assert_ptr_equal(scm_number_p(n), scm_true);
    assert_ptr_equal(scm_number_p(scm_true), scm_false);
    assert_int_equal(get_number_value((void*)n), 123);
}

static void test_scm_number_addition_without_arguments()
{
    struct scm_obj* result = scm_number_addition(scm_nil);
    int64_t n = get_number_value((void*)result);
    assert_int_equal(n, 0);
}

static void test_scm_number_addition_one_argument()
{
    struct scm_obj* args = scm_cons(create_number(123), scm_nil);
    struct scm_obj* result = scm_number_addition(args);
    int64_t n = get_number_value((void*)result);
    assert_int_equal(n, 123);
}

static void test_scm_number_addition_two_arguments()
{
    struct scm_obj* args = scm_cons(create_number(123), scm_cons(create_number(55), scm_nil));
    struct scm_obj* result = scm_number_addition(args);
    int64_t n = get_number_value((void*)result);
    assert_int_equal(n, 178);
}

int main(void)
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_scm_number),
        cmocka_unit_test(test_scm_number_addition_without_arguments),
        cmocka_unit_test(test_scm_number_addition_one_argument),
        cmocka_unit_test(test_scm_number_addition_two_arguments),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
