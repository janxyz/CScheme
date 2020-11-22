#ifndef CSCHEME_SYMBOL_H
#define CSCHEME_SYMBOL_H

#include "obj.h"

struct scm_symbol;

struct scm_obj const* scm_symbol_p(struct scm_obj const* const x);
struct scm_obj* create_symbol(char* str);

void init_symbol_table(void);
struct scm_obj* intern(char* str);
char* symbol_c_string(struct scm_symbol* symbol);

/* #define CSCHEME_SYMBOL_H */
#endif
