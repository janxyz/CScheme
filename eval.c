#include "eval.h"
#include "symbol.h"
#include "pair.h"
#include "string.h"
#include "number.h"
#include "procedure.h"
#include "utils.h"
#include "error.h"
// NULL
#include <stddef.h>

struct scm_obj* scm_eval(struct scm_obj* exp, struct scm_obj* const env)
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
            env
        );
    } else if (is_application(exp)) {
        struct scm_obj* op = scm_eval(application_operator(exp), env);
        struct scm_obj* args = eval_list(application_operands(exp), env);
        return apply(op, args);
    }
    exit_with_error("Unknown expression\n");
    return NULL;
}

struct scm_obj* eval_list(struct scm_obj* exp, struct scm_obj* const env)
{
    struct scm_obj* values = scm_nil;
    while (exp != scm_nil) {
        if (scm_pair_p(exp) == scm_true) {
            values = scm_cons(scm_eval(scm_car(exp), env), values);
            exp = scm_cdr(exp);
        } else {
            exit_with_error("Expected list argument\n");
        }
    }
    struct scm_obj* reversed = scm_nil;
    while (values != scm_nil) {
        reversed = scm_cons(scm_car(values), reversed);
        values = scm_cdr(values);
    }
    return reversed;
}

/**
 * Evaluate expressions in order and return value of last expression.
 */
struct scm_obj* eval_sequence(struct scm_obj* exp, struct scm_obj* const env)
{
    struct scm_obj* val = NULL;
    while (scm_pair_p(exp) == scm_true) {
        val = scm_eval(scm_car(exp), env);
        exp = scm_cdr(exp);
    }
    if (val == NULL) {
        exit_with_error("Empty expression list\n");
    }
    return val;
}

bool is_self_evaluating(struct scm_obj const* const exp)
{
    return (
        scm_boolean_p(exp) == scm_true ||
        scm_string_p(exp) == scm_true ||
        scm_number_p(exp) == scm_true
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

struct scm_obj* eval_if(struct scm_obj const* const exp, struct scm_obj* const env)
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
    return scm_false;
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

struct scm_obj* make_frame(struct scm_obj* variables, struct scm_obj* values)
{
    struct scm_obj* frame = scm_nil;
    while (scm_pair_p(variables) == scm_true && scm_pair_p(values) == scm_true) {
        struct scm_obj* binding = scm_cons(scm_car(variables), scm_car(values));
        frame = scm_cons(binding, frame);
        variables = scm_cdr(variables);
        values = scm_cdr(values);
    }
    if (variables != scm_nil || values != scm_nil) {
        exit_with_error("Uneven number of variables and values\n");
    }
    return frame;
}

bool is_application(struct scm_obj const* const exp)
{
    return scm_pair_p(exp) == scm_true;
}

struct scm_obj* apply(struct scm_obj* procedure, struct scm_obj* arguments)
{
    // Eval does not check if this actually a procedure
    if (scm_procedure_p(procedure) != scm_true) {
        exit_with_error("Non-procedure expression in application form\n");
    }
    if (is_primitive(procedure)) {
        primitive_function fn = primitive_procedure_function(procedure);
        return fn(arguments);
    } else {
        struct scm_obj* parameters = scm_procedure_parameters(procedure);
        struct scm_obj* body = scm_procedure_body(procedure);
        struct scm_obj* env = scm_procedure_environment(procedure);
        env = scm_cons(make_frame(parameters, arguments), env);
        return eval_sequence(body, env);
    }
}

struct scm_obj* application_operator(struct scm_obj const* const exp)
{
    return scm_car(exp);
}

struct scm_obj* application_operands(struct scm_obj const* const exp)
{
    return scm_cdr(exp);
}
