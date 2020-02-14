#ifndef MOPL_SYMBOL_H
#define MOPL_SYMBOL_H

#include "obj.h"

struct scm_symbol;

struct scm_obj const* scm_symbol_p(struct scm_obj const* const x);
struct scm_obj* create_symbol(char* str);

struct scm_obj* intern(struct scm_obj** symbol_table, char* str);

/* #define MOPL_SYMBOL_H */
#endif
