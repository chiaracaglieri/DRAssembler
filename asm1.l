%{
#include "symbols.h"
%}

%option noinput yylineno

%%
"loc" return I_OP_1;
"memloc"|"regval" return I_OP_2;
"clear"|"incr" return R_OP_1;
"add"|"sub"|"mul" return OP_ARITM;
"load"|"store" return OP_LS;
"memlocs" return I_OP_V;
"if<"|"if>"|"if=" return OP_COND;
"start" return START;
"end"   return END;
[R][0-9]* return REGISTER;
[a-zA-Z][_a-zA-Z0-9]*   return LABEL;
[0-9]*  return VALUE;
[$^, \t \n:]+ ;

%%

int yywrap(void){
    return 1;
}