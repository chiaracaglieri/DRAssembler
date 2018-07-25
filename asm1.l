%{
#include "symbols.h"
%}

%option noinput yylineno

%%
"loc"|"memloc"|"regval"|"clear"|"incr"|"add"|"sub"|"mul"|"load"|"store"|"memlocs"|"if<"|"if>"|"if=" return OPERATION;
"start" return START;
"end"   return END;
[R][0-9]* return REGISTER;
[a-zA-Z][_a-zA-Z0-9]*   return LABEL;
[0-9]*  return VALUE;
[\n] return I_END;
[$^, \t:]+ ;

%%

int yywrap(void){
    return 1;
}