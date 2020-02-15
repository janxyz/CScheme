#ifndef MOPL_EVAL_H
#define MOPL_EVAL_H

#include "obj.h"

struct scm_obj* eval(struct scm_obj* exp, struct scm_obj const* const env);

/* #define MOPL_EVAL_H */
#endif
