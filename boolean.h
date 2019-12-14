#ifndef MOPL_BOOLEAN_H
#define MOPL_BOOLEAN_H

#include "obj.h"

struct scm_boolean;

struct scm_obj const* const scm_false;
struct scm_obj const* const scm_true;

struct scm_obj const* scm_boolean_p(struct scm_obj const* const x);

/* #define MOPL_BOOLEAN_H */
#endif
