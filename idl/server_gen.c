#include <stdio.h>

#include "includes/queue.h"
#include "includes/parser.h"

void write_local (struct interface* interface) {
  struct method* method;
  struct arg* arg;
  int i, j, k;
  char filename[255] = {0}, u_ret_type[255] = {0};
  sprintf (filename, "%s/Local%s.java", dir_out, interface->name);
  remove (filename);
  FILE* file = fopen (filename, "a");

  fprintf (file, "package %s;\n\n", package);
  fprintf (file, "import org.simpledroidrpc.library.RPCServerInterface;\nimport org.simpledroidrpc.library.RPCServerRegistry;\n");
  fprintf (file, "import org.json.JSONArray;\nimport android.util.Log;\n\n");

  fprintf (file, "public class Local%s extends RPCServerInterface {\n",
      interface->name);
  fprintf (file, "private static %s obj = null;\n", interface->name);
  fprintf (file, "private static String TAG = \"SimpleDroidRPC.Local%s\";\n", interface->name);
  fprintf (file, "@Override\npublic void register () {\n");
  fprintf (file, "new RPCServerRegistry ().register (\"%s\", this);\n}\n", interface->name);
  fprintf (file, "@Override\npublic JSONArray call (String method, JSONArray args) {\n");
  fprintf (file, "if (obj == null)\nobj = new %s ();\n", interface->name);

  SIMPLEQ_FOREACH(method, &interface->methods, lh) {
    fprintf (file, "if (method.equals (\"%s\")) {\ntry {\n", method->name);
    
    i = 0;
    SIMPLEQ_FOREACH(arg, &method->args, lh) {
      strcpy (u_ret_type, method->ret_type);
      u_ret_type[0] = toupper (method->ret_type[0]);

      fprintf (file, "%s r%d = args.get%s(%d);\n", method->ret_type, i,
          u_ret_type, i);

      i++;
    }

    fprintf (file, "%s ret = obj.%s (", method->ret_type, method->name);

    k = 0;
    for (j=0; j<i; j++) {
      if (k == 0) k = 1;
      else        fprintf (file, ", ");
      fprintf (file, "r%d", j);
    }
    fprintf (file, ");\nJSONArray arr = new JSONArray ();\narr.put (ret);\nreturn arr;\n");
    fprintf (file, "} catch (Exception e) {\nLog.e (TAG, e.getMessage ());\n}\n");
    fprintf (file, "}\n");
  }
  
  fprintf (file, "return null;\n}\n}");
  fclose (file);
}

void write_server_main (struct interfaces_head* interfaces) {
  struct interface* interface;
  char filename[255] = {0};
  sprintf (filename, "%s/RPCServerInstance.java", dir_out);
  remove (filename);
  FILE* file = fopen (filename, "a");

  fprintf (file, "package %s;\n\n", package);
  fprintf (file, "import org.simpledroidrpc.library.RPCServer;\n");
  fprintf (file, "public class RPCServerInstance {\n");

  fprintf (file, "public RPCServerInstance (int port) {\n");
  fprintf (file, "new RPCServer (port);\n");
  fprintf (file, "register_interfaces ();\n}\n");
  
  fprintf (file, "public RPCServerInstance () {\n");
  fprintf (file, "new RPCServer ();\n");
  fprintf (file, "register_interfaces ();\n}\n");

  fprintf (file, "private void register_interfaces () {\n");
  
  SIMPLEQ_FOREACH(interface, interfaces, lh) {
    fprintf (file, "new Local%s ().register ();\n", interface->name);
  }

  fprintf (file, "}\n}");
}
