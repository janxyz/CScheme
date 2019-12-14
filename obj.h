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
    TYPE_NIL
};

#define scm_obj_header \
    int type

struct scm_obj {
    scm_obj_header;
};

struct scm_obj const* scm_eq_p(
    struct scm_obj const* const x,
    struct scm_obj const* const y);

/* MOPL_OBJ_H */
#endif
