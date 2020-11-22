#ifndef CSCHEME_PAIR_H
#define CSCHEME_PAIR_H

#include "obj.h"

struct scm_pair;

struct scm_obj const* scm_pair_p(struct scm_obj const* const x);
struct scm_obj* scm_cons(struct scm_obj* const x, struct scm_obj* const y);
struct scm_obj* scm_car(struct scm_obj const* const x);
struct scm_obj* scm_cdr(struct scm_obj const* const x);

/* #define CSCHEME_PAIR_H */
#endif
