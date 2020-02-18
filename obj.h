#ifndef MOPL_OBJ_H
#define MOPL_OBJ_H

enum {
    TYPE_SYMBOL,
    TYPE_NUMBER,
    TYPE_PAIR,
    TYPE_STRING,
    TYPE_BOOLEAN,
    TYPE_PORT,
    TYPE_EOF,
    TYPE_NIL,
    TYPE_PROCEDURE,
    TYPE_PRIMITIVE_PROCEDURE
};

#define scm_obj_header \
    int type

struct scm_obj {
    scm_obj_header;
};

struct scm_obj* const scm_nil;
struct scm_obj* const scm_false;
struct scm_obj* const scm_true;

struct scm_obj* scm_eq_p(struct scm_obj const* const x, struct scm_obj const* const y);
struct scm_obj* scm_null_p(struct scm_obj const* const x);
struct scm_obj* scm_boolean_p(struct scm_obj const* const x);

/* MOPL_OBJ_H */
#endif
