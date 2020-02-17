#include "eval.h"
#include "symbol.h"
#include "pair.h"
#include "string.h"
#include "procedure.h"
#include "utils.h"
#include "error.h"
// NULL
#include <stddef.h>

struct scm_obj* scm_eval(struct scm_obj* exp, struct scm_obj const* const env)
{
    if (is_self_evaluating(exp)) {
        return exp;
    } else if (is_variable(exp)) {
        return lookup_variable_value(exp, env);
    } else if (is_quote(exp)) {
        return quoted_exp(exp);
    } else if (is_if(exp)) {
        return eval_if(exp, env);
    } else if (is_lambda(exp)) {
        return scm_make_procedure(
            lambda_parameters(exp),
            lambda_body(exp),
            (void*)env
        );
    }
    exit_with_error("Unknown expression\n");
    return NULL;
}

bool is_self_evaluating(struct scm_obj const* const exp)
{
    return (
        scm_boolean_p(exp) == scm_true ||
        scm_string_p(exp) == scm_true ||
        scm_null_p(exp) == scm_true
    );
}

bool is_variable(struct scm_obj const* const exp)
{
    return scm_symbol_p(exp) == scm_true;
}

bool is_quote(struct scm_obj const* const exp)
{
    return scm_pair_p(exp) == scm_true && scm_car(exp) == intern("quote");
}

bool is_if(struct scm_obj const* const exp)
{
    return scm_pair_p(exp) == scm_true && scm_car(exp) == intern("if");
}

bool is_lambda(struct scm_obj const* const exp)
{
    return scm_pair_p(exp) == scm_true && scm_car(exp) == intern("lambda");
}

struct scm_obj* quoted_exp(struct scm_obj const* const exp)
{
    return scm_car(scm_cdr(exp));
}

struct scm_obj* eval_if(struct scm_obj const* const exp, struct scm_obj const* const env)
{
    struct scm_obj* test = scm_car(scm_cdr(exp));
    struct scm_obj* consequent = scm_car(scm_cdr(scm_cdr(exp)));
    struct scm_obj* alternate = scm_car(scm_cdr(scm_cdr(scm_cdr(exp))));
    if (scm_eval(test, env) != scm_false) {
        return scm_eval(consequent, env);
    } else {
        return scm_eval(alternate, env);
    }
}

struct scm_obj* lambda_parameters(struct scm_obj const* const exp)
{
    return scm_car(scm_cdr(exp));
}

struct scm_obj* lambda_body(struct scm_obj const* const exp)
{
    return scm_cdr(scm_cdr(exp));
}

/**
 * Returns the first binding found in list of frames whos car is eq? to exp.
 * Otherwise returns scm_false.
 */
struct scm_obj* lookup_variable_binding(struct scm_obj const* const exp, struct scm_obj const* env)
{
    while (scm_pair_p(env) == scm_true) {
        struct scm_obj* result = scm_assq(exp, scm_car(env));
        if (result == scm_false) {
            env = scm_cdr(env);
        } else {
            return result;
        }
    }
    return (void*)scm_false;
}

struct scm_obj* lookup_variable_value(struct scm_obj const* const exp, struct scm_obj const* env)
{
    struct scm_obj* binding = lookup_variable_binding(exp, env);
    if (scm_pair_p(binding) == scm_true) {
        return scm_cdr(binding);
    } else {
        exit_with_error("Unbound variable\n");
    }
    return NULL;
}
