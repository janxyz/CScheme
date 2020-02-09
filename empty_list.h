#ifndef MOPL_EMPTY_LIST_H
#define MOPL_EMPTY_LIST_H

#include "obj.h"

struct scm_empty_list;

struct scm_obj const* const scm_the_empty_list;

struct scm_obj const* scm_null_p(struct scm_obj const* const x);

/* #define MOPL_EMPTY_LIST_H */
#endif
