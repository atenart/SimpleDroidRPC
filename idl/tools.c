#include "includes/tools.h"
#include "includes/queue.h"
#include "includes/parser.h"

void add_interface (char* name) {
  struct interface* new = malloc (sizeof (struct interface));
  new->name = name;
  new->methods.sqh_first = NULL;
  new->methods.sqh_last = &new->methods.sqh_first;

  cur_interface = new;
  SIMPLEQ_INSERT_TAIL(&interfaces, new, lh);
}

void add_method (char* name) {
  struct method* new = malloc (sizeof (struct method));
  new->name = name;
  new->async = 0;
  new->args.sqh_first = NULL;
  new->args.sqh_last = &new->args.sqh_first;

  cur_method = new;
  
  SIMPLEQ_INSERT_TAIL(&cur_interface->methods, new, lh);
}

void add_arg (char* type, char *name) {
  struct arg* new = malloc (sizeof (struct arg));
  new->name = name;
  new->type = type;

  SIMPLEQ_INSERT_TAIL(&cur_method->args, new, lh);
}

void set_async () {
  cur_method->async = 1;
}

void set_return (char* type) {
  cur_method->ret_type = type;
}
