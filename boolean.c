#include "boolean.h"

struct scm_boolean {
    scm_obj_header;
};

struct scm_obj const* const scm_false =
    &(struct scm_obj){ .type = TYPE_BOOLEAN };

struct scm_obj const* const scm_true =
    &(struct scm_obj){ .type = TYPE_BOOLEAN };

struct scm_obj const* scm_boolean_p(struct scm_obj const* const x)
{
    return x->type == TYPE_BOOLEAN ? scm_true : scm_false;
}
