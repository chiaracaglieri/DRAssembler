%{
#include "asm1.tab.h"
extern int yylex();
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
[a-zA-Z][_a-zA-Z0-9]* return ID;
[0-9]*  return VALUE;
[:] return SEMICOLON;
[,] return COMMA;
[$^ \t\n]+ ;

%%