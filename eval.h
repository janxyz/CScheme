#ifndef MOPL_EVAL_H
#define MOPL_EVAL_H

#include "obj.h"
// bool
#include <stdbool.h>

struct scm_obj* scm_eval(struct scm_obj* exp, struct scm_obj* const env);

struct scm_obj* eval_list(struct scm_obj* exp, struct scm_obj* const env);
bool is_self_evaluating(struct scm_obj const* const exp);
bool is_variable(struct scm_obj const* const exp);
bool is_quote(struct scm_obj const* const exp);
bool is_if(struct scm_obj const* const exp);
bool is_lambda(struct scm_obj const* const exp);
struct scm_obj* quoted_exp(struct scm_obj const* const exp);
struct scm_obj* eval_if(struct scm_obj const* const exp, struct scm_obj* const env);
struct scm_obj* lambda_parameters(struct scm_obj const* const exp);
struct scm_obj* lambda_body(struct scm_obj const* const exp);
struct scm_obj* lookup_variable_binding(struct scm_obj const* const exp, struct scm_obj const* env);
struct scm_obj* lookup_variable_value(struct scm_obj const* const exp, struct scm_obj const* env);

/* #define MOPL_EVAL_H */
#endif
