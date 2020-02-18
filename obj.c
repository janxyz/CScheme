#include "obj.h"

struct scm_obj* const scm_nil = &(struct scm_obj){ .type = TYPE_NIL };
struct scm_obj* const scm_false = &(struct scm_obj){ .type = TYPE_BOOLEAN };
struct scm_obj* const scm_true = &(struct scm_obj){ .type = TYPE_BOOLEAN };

struct scm_obj* scm_eq_p(struct scm_obj const* const x, struct scm_obj const* const y)
{
    return (x == y) ? scm_true : scm_false;
}

struct scm_obj* scm_null_p(struct scm_obj const* const x)
{
    return x->type == TYPE_NIL ? scm_true : scm_false;
}

struct scm_obj* scm_boolean_p(struct scm_obj const* const x)
{
    return x->type == TYPE_BOOLEAN ? scm_true : scm_false;
}
