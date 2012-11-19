%{
#include <stdio.h>

#include "includes/parser.h"
#include "includes/queue.h"
#include "includes/tools.h"
#include "includes/server_gen.h"
#include "includes/client_gen.h"

int yylex ();

extern FILE* yyin;
%}

%token INTERFACE METHOD RETURNS ASYNC
%token ACC_OPEN ACC_CLOSE PAR_OPEN PAR_CLOSE COMMA
%token <str> STRING TYPE

%union {
    char* str;
}


%%

def : ACC_OPEN interfaces ACC_CLOSE;
interfaces : interface
           | interface interfaces;
interface : interface_name ACC_OPEN methods ACC_CLOSE;
interface_name : INTERFACE STRING { add_interface ($2); };
methods : method
        | method methods;
method : method_name PAR_OPEN args PAR_CLOSE RETURNS PAR_OPEN ret PAR_CLOSE method_attr;
method_name : METHOD STRING { add_method ($2); };
method_attr : ASYNC { set_async (); }
            | ;
args : arg
     | arg COMMA args;
arg : TYPE STRING { add_arg ($1, $2); };
    | ;
ret : TYPE { set_return ($1); };

%%

struct interfaces_head interfaces = SIMPLEQ_HEAD_INITIALIZER(interfaces);

int yyerror (const char* c) {
  fprintf (stderr, "error %s\n", c);
}

int main (int argc, char** argv) {
  struct interface* interface;
  struct method* method;
  FILE* file;

  if (argc != 4) {
    fprintf (stdout, "Usage: ./idl-compiler output_dir package_name interface_descriptor_file\n");
    return 0;
  }

  dir_out = argv[1];
  package = argv[2];
  file = fopen (argv[3], "r");
  if (!file) {
    fprintf (stderr, "Cannot open \"%s\"\n", argv[3]);
    return -1;
  }
  
  yyin = file;
 
  yyparse ();

  write_server_main (&interfaces);
  SIMPLEQ_FOREACH(interface, &interfaces, lh) {
    write_local (interface);
    write_remote (interface);
  }

  return 0;
}

