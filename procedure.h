#ifndef MOPL_PROCEDURE_H
#define MOPL_PROCEDURE_H

#include "obj.h"
#include <stdbool.h>

typedef struct scm_obj* (*primitive_function)(struct scm_obj* arguments);

bool is_primitive(struct scm_obj const* const x);

struct scm_obj const* scm_procedure_p(struct scm_obj const* const x);
struct scm_obj* scm_make_procedure(struct scm_obj* parameters, struct scm_obj* body, struct scm_obj* env);
struct scm_obj* scm_procedure_parameters(struct scm_obj const* const x);
struct scm_obj* scm_procedure_body(struct scm_obj const* const x);
struct scm_obj* scm_procedure_environment(struct scm_obj const* const x);

struct scm_obj* make_primitve_procedure(primitive_function function);
primitive_function primitive_procedure_function(struct scm_obj const* const x);

/* #define MOPL_PROCEDURE_H */
#endif
