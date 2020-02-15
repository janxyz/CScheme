#include "string.h"
// malloc
#include <stdlib.h>

struct scm_string {
    scm_obj_header;
    char* str;
};

struct scm_obj const* scm_string_p(struct scm_obj const* const x)
{
    return x->type == TYPE_STRING ? scm_true : scm_false;
}

struct scm_obj* create_string(char* str)
{
    struct scm_string* s = malloc(sizeof(*s));
    s->type = TYPE_STRING;
    s->str = str;
    return (void*)s;
}

char* c_string(struct scm_string* s)
{
    return s->str;
}
