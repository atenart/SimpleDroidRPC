#ifndef __SIMPLEDROIDRPC_TOOLS_H__
#define __SIMPLEDROIDRPC_TOOLS_H__

void add_interface (char* name);
void add_method (char* name);
void add_arg (char* type, char *name);
void set_async ();
void set_return (char* type);

#endif  /* __SIMPLEDROIDRPC_TOOLS_H__ */
