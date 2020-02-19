#include <stdio.h>
#include <stdbool.h>
#include "obj.h"
#include "pair.h"
#include "symbol.h"
#include "port.h"
#include "read.h"
#include "eval.h"
#include "procedure.h"

void print_helper(struct scm_obj* obj, bool is_cdr)
{
    if (!obj) {
        return;
    } else if (obj->type == TYPE_SYMBOL) {
        printf("%s", symbol_c_string((void*)obj));
    } else if (obj->type == TYPE_BOOLEAN) {
        printf("%s", obj == scm_true ? "#t" : "#f");
    } else if (obj->type == TYPE_PROCEDURE || obj->type == TYPE_PRIMITIVE_PROCEDURE) {
        printf("#<procedure>");
    } else if (obj->type == TYPE_PAIR) {
        struct scm_obj* car = scm_car(obj);
        struct scm_obj* cdr = scm_cdr(obj);
        if (!is_cdr) {
            printf("(");
        }
        print_helper(car, false);
        /* print a dot unless cdr is another pair */
        if (cdr->type != TYPE_PAIR && cdr->type != TYPE_NIL) {
            printf(" .");
        }
        if (cdr->type != TYPE_NIL) {
            printf(" ");
        }
        print_helper(cdr, true);
        if (!is_cdr) {
            printf(")");
        }
    }
}

void print_exp(struct scm_obj* obj)
{
    print_helper(obj, false);
}

struct scm_obj* prim_scm_eq_p(struct scm_obj* const args)
{
    return scm_eq_p(scm_car(args), scm_car(scm_cdr(args)));
}

int main(void)
{
    printf("mopl 1.0\n");
    init_symbol_table();
    init_ports();

    // Setup environment
    struct scm_obj* frame = scm_nil;
    frame = scm_cons(scm_cons(intern("eq?"), make_primitve_procedure(&prim_scm_eq_p)), frame);
    struct scm_obj* env = scm_cons(frame, scm_nil);

    struct scm_obj* port = scm_current_input_port();
    struct scm_obj* exp = NULL;
    while (true) {
        printf("> ");
        exp = scm_read(port);
        print_exp(scm_eval(exp, env));
        printf("\n");
    }
}
