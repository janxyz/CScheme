#include "number.h"
#include "pair.h"
#include "error.h"
// malloc
#include <stdlib.h>
#include <stdint.h>

struct scm_number {
    scm_obj_header;
    int64_t value;
};

struct scm_obj const* scm_number_p(struct scm_obj const* const x)
{
    return x->type == TYPE_NUMBER ? scm_true : scm_false;
}

struct scm_obj* scm_number_addition(struct scm_obj const* const args)
{
    int64_t sum = 0;
    struct scm_obj const* list = args;
    struct scm_obj* car;

    while (scm_pair_p(list) == scm_true) {
        car = scm_car(list);
        if (scm_number_p(car) == scm_false) {
            exit_with_error("Addition of non-number object\n");
        }
        sum += get_number_value((void*)car);
        list = scm_cdr(list);
    }

    if (list != scm_nil) {
        exit_with_error("Addition of non-number object\n");
    }

    return create_number(sum);
}

struct scm_obj* create_number(int64_t value)
{
    struct scm_number* n = malloc(sizeof(*n));
    n->type = TYPE_NUMBER;
    n->value = value;
    return (void*)n;
}

int64_t get_number_value(struct scm_number* n)
{
    return n->value;
}
