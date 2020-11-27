#ifndef CSCHEME_NUMBER_H
#define CSCHEME_NUMBER_H

#include "obj.h"
#include <stdint.h>

struct scm_number;

struct scm_obj const* scm_number_p(struct scm_obj const* const x);
struct scm_obj* scm_number_addition(struct scm_obj const* const args);

struct scm_obj* create_number(int64_t value);
int64_t get_number_value(struct scm_number* n);

/* #define CSCHEME_NUMBER_H */
#endif
