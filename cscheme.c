#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "obj.h"
#include "pair.h"
#include "symbol.h"
#include "string.h"
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
    } else if (obj->type == TYPE_STRING) {
        printf("\"%s\"", c_string((void*)obj));
    } else if (obj->type == TYPE_NIL && !is_cdr) {
        printf("()");
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

struct scm_obj* prim_scm_cons(struct scm_obj* const args)
{
    return scm_cons(scm_car(args), scm_car(scm_cdr(args)));
}

struct scm_obj* prim_scm_car(struct scm_obj* const args)
{
    return scm_car(scm_car(args));
}

struct scm_obj* prim_scm_cdr(struct scm_obj* const args)
{
    return scm_cdr(scm_car(args));
}

struct scm_obj* prim_scm_eval(struct scm_obj* const args)
{
    struct scm_obj* exp = scm_car(args);
    struct scm_obj* env = scm_car(scm_cdr(args));
    return scm_eval(exp, env);
}

struct scm_obj* env = NULL;

struct scm_obj* prim_scm_interaction_environment(struct scm_obj* const args)
{
    (void)args;
    return env;
}

int main(int argc, char** argv)
{
    // Default mode when no arguments are given
    bool interactive = true;
    char* input_file = NULL;

    if (argc == 2) {
        if (strcmp(argv[0], "-i") != 0) {
            interactive = false;
            input_file = argv[1];
        }
    } else if (argc == 3) {
        if (strcmp(argv[1], "-i") == 0) {
            input_file = argv[2];
        } else {
            interactive = false;
            input_file = argv[1];
        }
    }

    init_symbol_table();
    init_ports();

    // Setup environment
    struct scm_obj* frame = scm_nil;
    frame = scm_cons(scm_cons(intern("eq?"), make_primitve_procedure(&prim_scm_eq_p)), frame);
    frame = scm_cons(scm_cons(intern("cons"), make_primitve_procedure(&prim_scm_cons)), frame);
    frame = scm_cons(scm_cons(intern("car"), make_primitve_procedure(&prim_scm_car)), frame);
    frame = scm_cons(scm_cons(intern("cdr"), make_primitve_procedure(&prim_scm_cdr)), frame);
    frame = scm_cons(scm_cons(intern("eval"), make_primitve_procedure(&prim_scm_eval)), frame);
    frame = scm_cons(scm_cons(intern("interaction-environment"), make_primitve_procedure(&prim_scm_interaction_environment)), frame);
    env = scm_cons(frame, scm_nil);

    // Evaluate input file
    if (input_file != NULL) {
        struct scm_obj* input_file_string = create_string(input_file);
        struct scm_obj* input_file_port = scm_open_input_file(input_file_string);
        struct scm_obj* exp = scm_read(input_file_port);
        print_exp(scm_eval(exp, env));
        printf("\n");
    }

    // Run REPL
    if (interactive) {
        while (true) {
            char* input = readline("> ");
            if (!input) {
                printf("\nBye\n");
                break;
            }
            add_history(input);
            struct scm_obj* port = scm_open_input_string(create_string(input));
            struct scm_obj* exp;
            while ((exp = scm_read(port)) != NULL) {
                print_exp(scm_eval(exp, env));
                printf("\n");
            }
            free(input);
        }
    }
}
