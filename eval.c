#include "eval.h"
#include "symbol.h"
#include "pair.h"
#include "string.h"
#include "utils.h"
#include "error.h"
#include "utils.h"
// NULL
#include <stddef.h>

struct scm_obj* env_binding(struct scm_obj const* env, struct scm_obj const* const symbol);

struct scm_obj* eval(struct scm_obj* exp, struct scm_obj const* const env)
{
    if (scm_boolean_p(exp) == scm_true ||
        scm_string_p(exp) == scm_true ||
        scm_null_p(exp) == scm_true) {
        return exp;
    } else if (scm_symbol_p(exp) == scm_true) {
        struct scm_obj* binding = env_binding(exp, env);
        if (scm_pair_p(binding) == scm_true) {
            return scm_cdr(binding);
        } else {
            exit_with_error("Unbound variable\n");
        }
    }
    exit_with_error("Unknown expression\n");
    return NULL;
}

/**
 * Returns the first binding found in list of frames whos car is symbol.
 * Otherwise returns scm_false.
 */

struct scm_obj* env_binding(struct scm_obj const* const symbol, struct scm_obj const* env)
{
    while (scm_pair_p(env) == scm_true) {
        struct scm_obj* result = scm_assq(symbol, scm_car(env));
        if (result == scm_false) {
            env = scm_cdr(env);
        } else {
            return result;
        }
    }
    return (void*)scm_false;
}
