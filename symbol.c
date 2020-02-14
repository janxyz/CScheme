#include "symbol.h"
#include "boolean.h"
#include "pair.h"
#include "empty_list.h"
#include "error.h"

// malloc
#include <stdlib.h>
// strcmp
#include <string.h>

struct scm_symbol {
    scm_obj_header;
    char* str;
};

struct scm_obj const* scm_symbol_p(struct scm_obj const* const x)
{
    return x->type == TYPE_SYMBOL ? scm_true : scm_false;
}

struct scm_obj* create_symbol(char* str)
{
    struct scm_symbol* s = malloc(sizeof(*s));
    s->type = TYPE_SYMBOL;
    s->str = str;
    return (void*)s;
}

struct scm_obj* intern(struct scm_obj** symbol_table, char* str)
{
    struct scm_obj* list = *symbol_table;
    while (scm_pair_p(list) == scm_true) {
        struct scm_obj* car = scm_car(list);
        if (car->type == TYPE_SYMBOL) {
            struct scm_symbol* sym = (void*)car;
            if (strcmp(sym->str, str) == 0) {
                return car;
            }
        } else {
            exit_with_error("Non-Symbol in symbol table\n");
        }
        list = scm_cdr(list);
    }
    // not a proper list
    if (list != scm_the_empty_list) {
        exit_with_error("Invaid symbol table\n");
    }
    // Add new symbol to symbol table
    struct scm_obj* sym = create_symbol(str);
    *symbol_table = scm_cons(sym, *symbol_table);
    return sym;
}
