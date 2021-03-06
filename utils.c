#include "utils.h"
#include "obj.h"
#include "pair.h"
#include "error.h"

struct scm_obj* scm_assq(struct scm_obj const* const key, struct scm_obj const* alist)
{
    while (scm_pair_p(alist) == scm_true) {
        struct scm_obj* binding = scm_car(alist);
        if (scm_pair_p(binding) == scm_true) {
            if (scm_eq_p(scm_car(binding), key) == scm_true) {
                return binding;
            }
        } else {
            exit_with_error("Improper association list\n");
        }
        alist = scm_cdr(alist);
    }
    // End of list but not empty list
    if (scm_null_p(alist) != scm_true) {
        exit_with_error("Improper association list\n");
    }
    return scm_false;
}
