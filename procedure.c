#include "procedure.h"
#include "error.h"
// malloc
#include <stdlib.h>

struct scm_procedure {
    scm_obj_header;
    struct scm_obj* parameters;
    struct scm_obj* body;
    struct scm_obj* env;
};

struct scm_primitive_procedure {
    scm_obj_header;
    primitive_function function;
};

bool is_primitive(struct scm_obj const* const x)
{
    return x->type == TYPE_PRIMITIVE_PROCEDURE;
}

struct scm_obj const* scm_procedure_p(struct scm_obj const* const x)
{
    return (x->type == TYPE_PROCEDURE || x->type == TYPE_PRIMITIVE_PROCEDURE) ? scm_true : scm_false;
}

struct scm_obj* scm_make_procedure(struct scm_obj* parameters, struct scm_obj* body, struct scm_obj* env)
{
    struct scm_procedure* p = malloc(sizeof(*p));
    p->type = TYPE_PROCEDURE;
    p->parameters = parameters;
    p->body = body;
    p->env = env;
    return (void*)p;
}

struct scm_obj* scm_procedure_parameters(struct scm_obj const* const x)
{
    if (x->type != TYPE_PROCEDURE) {
        exit_with_error("Type error: procedure expected\n");
    }
    return ((struct scm_procedure*)x)->parameters;
}

struct scm_obj* scm_procedure_body(struct scm_obj const* const x)
{
    if (x->type != TYPE_PROCEDURE) {
        exit_with_error("Type error: procedure expected\n");
    }
    return ((struct scm_procedure*)x)->body;
}

struct scm_obj* scm_procedure_environment(struct scm_obj const* const x)
{
    if (x->type != TYPE_PROCEDURE) {
        exit_with_error("Type error: procedure expected\n");
    }
    return ((struct scm_procedure*)x)->env;
}

struct scm_obj* make_primitive_procedure(primitive_function function)
{
    struct scm_primitive_procedure* p = malloc(sizeof(*p));
    p->type = TYPE_PRIMITIVE_PROCEDURE;
    p->function = function;
    return (void*)p;
}

primitive_function primitive_procedure_function(struct scm_obj const* const x)
{
    if (!is_primitive(x)) {
        exit_with_error("Type error: primitive procedure expected\n");
    }
    return ((struct scm_primitive_procedure*)x)->function;
}
