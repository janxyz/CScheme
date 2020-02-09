#include "empty_list.h"
#include "boolean.h"

struct scm_empty_list {
    scm_obj_header;
};

struct scm_obj const* const scm_the_empty_list =
    &(struct scm_obj){ .type = TYPE_NIL };

struct scm_obj const* scm_null_p(struct scm_obj const* const x)
{
    return x->type == TYPE_NIL ? scm_true : scm_false;
}
