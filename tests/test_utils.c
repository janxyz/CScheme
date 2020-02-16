// required for cmocka
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include "../utils.h"
#include "../pair.h"
#include "../symbol.h"
#include "../string.h"

// Mocks
void __wrap_exit_with_error(char const* format, ...)
{
    check_expected(format);
}

static void test_scm_assq_key_found(void** state)
{
    (void)state;
    struct scm_obj* symbol_table = (void*)scm_nil;
    struct scm_obj* key1 = intern(&symbol_table, "key-1");
    struct scm_obj* key2 = intern(&symbol_table, "key-2");
    struct scm_obj* pair1 = scm_cons(key1, (void*)scm_true);
    struct scm_obj* pair2 = scm_cons(key2, (void*)scm_false);
    struct scm_obj* alist = (void*)scm_nil;
    alist = scm_cons(pair1, alist);
    alist = scm_cons(pair2, alist);
    assert_true(scm_assq(key1, alist) == pair1);
    assert_true(scm_assq(key2, alist) == pair2);
}

static void test_scm_assq_no_key_found(void** state)
{
    (void)state;
    struct scm_obj* key = create_string("key-1");
    struct scm_obj* pair = scm_cons(key, key);
    struct scm_obj* alist = scm_cons(pair, (void*)scm_nil);
    assert_true(scm_assq(pair, alist) == scm_false);
    assert_true(scm_assq(key, (void*)scm_nil) == scm_false);
}

static void test_scm_assq_improper_alist(void** state)
{
    (void)state;
    expect_string(__wrap_exit_with_error, format, "Improper association list\n");
    scm_assq(scm_true, scm_true);
}

int main(void)
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_scm_assq_key_found),
        cmocka_unit_test(test_scm_assq_no_key_found),
        cmocka_unit_test(test_scm_assq_improper_alist),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
