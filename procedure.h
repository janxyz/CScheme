#ifndef MOPL_PROCEDURE_H
#define MOPL_PROCEDURE_H

#include "obj.h"

struct scm_procedure;

struct scm_obj const* scm_procedure_p(struct scm_obj const* const x);
struct scm_obj* scm_make_procedure(struct scm_obj* parameters, struct scm_obj* body, struct scm_obj* env);
struct scm_obj* scm_procedure_parameters(struct scm_obj const* const x);
struct scm_obj* scm_procedure_body(struct scm_obj const* const x);
struct scm_obj* scm_procedure_environment(struct scm_obj const* const x);

/* #define MOPL_PROCEDURE_H */
#endif
