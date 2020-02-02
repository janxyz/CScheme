#ifndef MOPL_STRING_H
#define MOPL_STRING_H

#include "obj.h"

struct scm_string;

struct scm_obj const* scm_string_p(struct scm_obj const* const x);

struct scm_obj* create_string(char* str);
char* c_string(struct scm_string* s);

/* #define MOPL_STRING_H */
#endif
