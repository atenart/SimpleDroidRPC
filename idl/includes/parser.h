#ifndef __SIMPLEDROIDRPC_PARSER_H__
#define __SIMPLEDROIDRPC_PARSER_H__

#include "queue.h"

char* dir_out;
char* package;

struct interface {
  SIMPLEQ_ENTRY(interfaces_head) lh;
  char* name;
  SIMPLEQ_HEAD(methods_head, method) methods;
};

struct method {
  SIMPLEQ_ENTRY(methods_head) lh;
  char* name;
  char* ret_type;
  int async;
  SIMPLEQ_HEAD(args_head, arg) args;
};

struct arg {
  SIMPLEQ_ENTRY(args_head) lh;
  char* type;
  char* name;
};

SIMPLEQ_HEAD(interfaces_head, interface);

struct interface* cur_interface;
struct method* cur_method;

extern struct interfaces_head interfaces;

#endif  /* __SIMPLEDROIDRPC_PARSER_H__ */
