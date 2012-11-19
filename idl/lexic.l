%{
#include "parser.tab.h"
%}


%%

[ \t\n]       ;
interface     { return INTERFACE; }
method        { return METHOD; }
returns       { return RETURNS; }
async         { return ASYNC; }
"int"|"bool"  { yylval.str = strdup(yytext); return TYPE; }
"String"      { yylval.str = strdup(yytext); return TYPE; }
[a-zA-Z0-9]+  { yylval.str = strdup(yytext); return STRING; }
"{"           { return ACC_OPEN; }
"}"           { return ACC_CLOSE; }
"("           { return PAR_OPEN; }
")"           { return PAR_CLOSE; }
","           { return COMMA; }

%%

int yywrap () {
  return 1;
}
