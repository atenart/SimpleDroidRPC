#include <stdio.h>

#include "includes/queue.h"
#include "includes/parser.h"

void write_sync_method (FILE* file, struct interface* interface,
    struct method* method) {
  struct arg* arg;
  int i;
  
  fprintf (file, "public %s %s (", method->ret_type, method->name);

  i = 0;
  SIMPLEQ_FOREACH(arg, &method->args, lh) {
    if (i == 0) i = 1;
    else        fprintf (file, ", ");
    fprintf (file, "%s %s", arg->type, arg->name);
  }

  fprintf (file, ") throws SimpleDroidRPCException ");
  fprintf (file, "{\nJSONArray args = new JSONArray ();\n");

  if (strcmp (method->ret_type, "String") == 0)
    fprintf (file, "String ret = \"\";\n");
  else if (strcmp (method->ret_type, "int") == 0)
    fprintf (file, "int ret = 0;\n");
  else if (strcmp (method->ret_type, "boolean") == 0)
    fprintf (file, "boolean ret = false;\n");

  SIMPLEQ_FOREACH(arg, &method->args, lh) {
    fprintf (file, "args.put (%s);\n", arg->name);
  }

  fprintf (file, "JSONArray rep = invoque (\"%s\", \"%s\", args);\ntry{\n", interface->name, method->name);

  if (strcmp (method->ret_type, "String") == 0)
    fprintf (file, "ret = rep.getString (0);\n");
  else if (strcmp (method->ret_type, "int") == 0)
    fprintf (file, "ret = rep.getInt (0);\n");
  else if (strcmp (method->ret_type, "boolean") == 0)
    fprintf (file, "ret = rep.getBoolean (0);\n");

  fprintf (file, "} catch (Exception e) {\nLog.e (TAG, e.getMessage ());\n");
  fprintf (file, "throw new SimpleDroidRPCException (\"Communication error\");");
  fprintf (file, "\n}\nreturn ret;\n}\n");
}

void write_async_method (FILE* file, struct interface* interface,
    struct method* method) {
  struct arg* arg;
  char arg_class[50] = {0}, method_class[255] = {0};

  if (strcmp (method->ret_type, "String") == 0)
    sprintf (arg_class, "String");
  else if (strcmp (method->ret_type, "int") == 0)
    sprintf (arg_class, "Integer");
  else if (strcmp (method->ret_type, "boolean") == 0)
    sprintf (arg_class, "Boolean");

  strcpy (method_class, method->name);
  method_class[0] = toupper (method_class[0]);
  
  fprintf (file, "public void %s (", method->name);
  
  SIMPLEQ_FOREACH(arg, &method->args, lh) {
    fprintf (file, "%s %s, ", arg->type, arg->name);
  }

  fprintf (file, "InvocationListener<%s> listener) {\n", arg_class); 
  fprintf (file, "new %s (", method_class);
  
  SIMPLEQ_FOREACH(arg, &method->args, lh) {
    fprintf (file, "%s, ", arg->name);
  }

  fprintf (file, "listener);\n}\n");

  fprintf (file, "private class %s implements Runnable {\n", method_class);
  fprintf (file, "private InvocationListener<%s> listener;\n", arg_class);
  fprintf (file, "JSONArray args = null;\npublic %s (", method_class);

  SIMPLEQ_FOREACH(arg, &method->args, lh) {
    fprintf (file, "%s %s, ", arg->type, arg->name);
  }

  fprintf (file, "InvocationListener<%s> listener) {\n", arg_class);
  fprintf (file, "args = new JSONArray ();\n");

  SIMPLEQ_FOREACH(arg, &method->args, lh) {
    fprintf (file, "args.put (%s);\n", arg->name);
  }

  fprintf (file, "this.listener = listener;\n");
  fprintf (file, "Thread th = new Thread (this);\nth.start ();\n}\n");

  fprintf (file, "public void run () {\n");
  fprintf (file, "JSONArray rep = invoque (\"%s\", \"%s\", args);\n", interface->name, method->name);
  fprintf (file, "try {\nlistener.onComplete (");
  
  if (strcmp (method->ret_type, "String") == 0)
    fprintf (file, "rep.getString (0)");
  else if (strcmp (method->ret_type, "int") == 0)
    fprintf (file, "rep.getInt(0)");
  else if (strcmp (method->ret_type, "boolean") == 0)
    fprintf (file, "rep.getBoolean (0)");
  
  fprintf (file, ");\n} catch (Exception e) {\n");
  fprintf (file, "Log.e (TAG, e.getMessage ());\nlistener.onException (e);\n");
  
  fprintf (file, "}\n}\n}\n");
}

void write_remote (struct interface* interface) {
  struct method* method;
  char filename[255] = {0};
  sprintf (filename, "%s/Remote%s.java", dir_out, interface->name);
  remove (filename);
  FILE* file = fopen (filename, "a");

  fprintf (file, "package %s;\n\n", package);
  fprintf (file, "import org.simpledroidrpc.library.RPCClientInterface;\n");
  fprintf (file, "import org.simpledroidrpc.library.SimpleDroidRPCException;\n");
  fprintf (file, "import org.simpledroidrpc.library.InvocationListener;\n");
  fprintf (file, "import org.json.JSONArray;\nimport android.util.Log;\n\n");
  fprintf (file, "public class Remote%s extends RPCClientInterface {\n",
      interface->name);
  fprintf (file, "private static String TAG = \"Remote%s\";\n", interface->name);
  fprintf (file, "public Remote%s (String server) {\nsuper (server);\n}\n\n",
      interface->name);
  fprintf (file, "public Remote%s (String server, int port) {\nsuper (server, port);\n}\n\n",
      interface->name);
  fprintf (file, "public Remote%s (String server, int port, int timeout) {\nsuper (server, port, timeout);\n}\n\n",
      interface->name);

  SIMPLEQ_FOREACH(method, &interface->methods, lh) {
    if (method->async)
      write_async_method (file, interface, method);
    else
      write_sync_method (file, interface, method);
    
  }
  
  fprintf (file, "}");
  fclose (file);
}
