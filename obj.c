#include "obj.h"
#include "boolean.h"

struct scm_obj const*
scm_eq_p(
    struct scm_obj const* const x,
    struct scm_obj const* const y)
{
    return (x == y) ? scm_true : scm_false;
}
