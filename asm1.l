%{
#include "symbols.h"
%}

%option noinput yylineno

%%
"add"|"sub"|"mul"|"incr"|"load"|"store"|"memloc"|"loc"|"if<"|"regval"|"clear" return OPERATION;
"start" return START;
"end"   return END;
[a-zA-Z][_a-zA-Z0-9]*   return LABEL;
[0-9]*  return VALUE;
[$^, \t \n:]+ ;



%%

int yywrap(void){
    return 1;
}