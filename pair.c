#include "pair.h"
#include "boolean.h"
#include "error.h"
// NULL
#include <stddef.h>
// malloc
#include <stdlib.h>


struct scm_pair {
    scm_obj_header;
    struct scm_obj* car;
    struct scm_obj* cdr;
};

struct scm_obj const* scm_pair_p(struct scm_obj const* const x) {
    return x->type == TYPE_PAIR ? scm_true : scm_false;
}

struct scm_obj* scm_cons(struct scm_obj* const car, struct scm_obj* const cdr) {
    struct scm_pair* p = malloc(sizeof(*p));
    p->type = TYPE_PAIR;
    p->car = car;
    p->cdr = cdr;
    return (void*)p;
}

struct scm_obj* scm_car(struct scm_obj const* const x) {
    if (x->type != TYPE_PAIR) {
        /* exit(0); */
        exit_with_error("Calling car on non-pair object\n");
    }
    return ((struct scm_pair*)x)->car;
}

struct scm_obj* scm_cdr(struct scm_obj const* const x) {
    if (x->type != TYPE_PAIR) {
        exit_with_error("Calling cdr on non-pair object\n");
    }
    return ((struct scm_pair*)x)->cdr;
}
