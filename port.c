#include "port.h"
#include "string.h"
#include "error.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
// strlen
#include <string.h>

enum port_type {
    FILE_INPUT,
    FILE_OUTPUT,
    STRING_INPUT,
    STRING_OUTPUT
};

struct scm_port {
    scm_obj_header;
    enum port_type port_type;
    union {
        struct {
            FILE* file;
            char* buffer;
            size_t buffer_size;
        };
        struct {
            char* string;
            size_t len;
            size_t index;
        };
    };
};

static struct scm_port current_input_port;
static struct scm_port current_output_port;

struct scm_obj* scm_port_p(struct scm_obj const* const x)
{
    return x->type == TYPE_PORT ? scm_true : scm_false;
}

struct scm_obj* scm_input_port_p(struct scm_obj const* const x)
{
    if (x->type == TYPE_PORT) {
        enum port_type pt = ((struct scm_port*)x)->port_type;
        if (pt == FILE_INPUT || pt == STRING_INPUT) {
            return scm_true;
        }
    }
    return scm_false;
}

struct scm_obj* scm_output_port_p(struct scm_obj const* const x)
{
    if (x->type == TYPE_PORT) {
        enum port_type pt = ((struct scm_port*)x)->port_type;
        if (pt == FILE_OUTPUT || pt == STRING_OUTPUT) {
            return scm_true;
        }
    }
    return scm_false;
}

struct scm_obj* scm_port_closed_p(struct scm_obj const* x)
{
    if (x->type == TYPE_PORT) {
        struct scm_port* port = (void*)x;
        if (port->port_type == FILE_INPUT || port->port_type == FILE_OUTPUT) {
            return port->file == NULL ? scm_true : scm_false;
        } else if (port->port_type == STRING_INPUT || port->port_type == STRING_OUTPUT) {
            return port->string == NULL ? scm_true : scm_false;
        }
    }

    exit_with_error("Type error: port expected\n");
    return NULL;
}

struct scm_obj* scm_open_input_file(struct scm_obj const* const x)
{
    if (x->type != TYPE_STRING) {
        exit_with_error("Type error: string expected\n");
    }
    char* filename = c_string((void*)x);
    struct scm_port* p = malloc(sizeof(*p));
    p->type = TYPE_PORT;
    p->port_type = FILE_INPUT;
    p->file = fopen(filename, "r");
    if (!p->file) {
        exit_with_error("Unable to open file: %s\n", filename);
    }
    p->buffer = NULL;
    p->buffer_size = 0;
    return (void*)p;
}

struct scm_obj* scm_open_input_string(struct scm_obj const* const x)
{
    if (x->type != TYPE_STRING) {
        exit_with_error("Type error: string expected\n");
    }
    struct scm_port* p = malloc(sizeof(*p));
    p->type = TYPE_PORT;
    p->port_type = STRING_INPUT;
    p->string = c_string((void*)x);
    p->len = strlen(p->string);
    p->index = 0;
    return (void*)p;
}

struct scm_obj* scm_close_port(struct scm_obj* x)
{
    if (x->type == TYPE_PORT) {
        struct scm_port* p = (void*)x;
        if (p->port_type == FILE_INPUT || p->port_type == FILE_OUTPUT) {
            if (p->file) {
                // TODO: Check errors
                fclose(p->file);
                p->file = NULL;
            }
        } else if (p->port_type == STRING_INPUT || p->port_type == STRING_OUTPUT) {
            p->string = NULL;
        }
    } else {
        exit_with_error("Type error: port expected\n");
    }
    return x;
}

struct scm_obj* scm_current_input_port(void)
{
  return (void*)&current_input_port;
}

struct scm_obj* scm_current_output_port(void)
{
    return (void*)&current_output_port;
}

void init_ports(void)
{
    current_input_port.type = TYPE_PORT;
    current_input_port.port_type = FILE_INPUT;
    current_input_port.file = stdin;
    current_input_port.buffer = NULL;
    current_input_port.buffer_size = 0;

    current_output_port.type = TYPE_PORT;
    current_output_port.port_type = FILE_OUTPUT;
    current_output_port.file = stdout;
    current_output_port.buffer = NULL;
    current_output_port.buffer_size = 0;
}

int read_char(struct scm_port* port)
{
    if (scm_port_closed_p((void*)port) == scm_false) {
        if (port->port_type == FILE_INPUT) {
            return getc(port->file);
        } else if (port->port_type == STRING_INPUT) {
            if (port->index < port->len) {
                int c = port->string[port->index];
                port->index += 1;
                return c;
            } else {
                return EOF;
            }
        } else {
            exit_with_error("Read operation on non-input port\n");
            return -1;
        }
    }
    exit_with_error("Operation on closed port\n");
    return -1;
}

int peek_char(struct scm_port* port)
{
    if (scm_port_closed_p((void*)port) == scm_false) {
        if (port->port_type == FILE_INPUT) {
            int c = read_char(port);
            ungetc(c, port->file);
            return c;
        } else if (port->port_type == STRING_INPUT) {
            if (port->index < port->len) {
                return port->string[port->index];
            } else {
                return EOF;
            }
        } else {
            exit_with_error("Read operation on non-input port\n");
            return -1;
        }
    }
    exit_with_error("Operation on closed port\n");
    return -1;
}
