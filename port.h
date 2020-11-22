#ifndef CSCHEME_PORT_H
#define CSCHEME_PORT_H

#include "obj.h"

struct scm_port;

struct scm_obj* scm_port_p(struct scm_obj const* x);
struct scm_obj* scm_input_port_p(struct scm_obj const* x);
struct scm_obj* scm_output_port_p(struct scm_obj const* x);
struct scm_obj* scm_port_closed_p(struct scm_obj const* x);
struct scm_obj* scm_open_input_file(struct scm_obj const* x);
struct scm_obj* scm_open_input_string(struct scm_obj const* x);
struct scm_obj* scm_close_port(struct scm_obj* port);
struct scm_obj* scm_current_input_port(void);
struct scm_obj* scm_current_output_port(void);

void init_ports(void);
int read_char(struct scm_port* port);
int peek_char(struct scm_port* port);

/* #define CSCHEME_PORT_H */
#endif
