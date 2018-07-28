%{
#include "asm1.tab.h"
#include "symbols.h"
%}

%option noyywrap
%option noinput nounput

%%
"loc" return LOC;
"memloc" return MEMLOC;
"memlocs" return MEMLOCS;
"regval" return REGVAL;
"clear" return CLEAR;
"incr" return INCR;
"add" return ADD;
"sub" return SUB;
"mul" return MUL;
"load" return LOAD;
"store" return STORE;
"if=" return EQ;
"if>" return GT;
"if<" return LT;
"start" return START;
"end"   return END;
[R][0-9]* return REG;
[a-zA-Z][_a-zA-Z0-9]*   {last_string=malloc(sizeof(char)*strlen(yytext));
                         strcpy(last_string,yytext);
                         return ID;
                         }
[0-9]*  {last_value=atoi(yytext); return VALUE; }
[:] return SEMICOLON;
[,] return COMMA;
[$^ \t\n]+ ;

%%