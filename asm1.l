%{
#include <string>
#include "util.h"
#include "asm1.tab.h"
%}

%option noyywrap
%option noinput nounput

%%
"loc" return LOC;
"memloc" return MEMLOC;
"memlocs" return MEMLOCS;
"regval" return REGVAL;
"clear" return CLEAR;
"decr"  return DECR;
"incr" return INCR;
"add" return ADD;
"sub" return SUB;
"mul" return MUL;
"load" return LOAD;
"store" return STORE;
"if=" return EQ;
"if>" return GT;
"if<" return LT;
"goto" return GOTO;
"start" return START;
"end"   return END;
[R][0-9]* { tmp=yytext;
            tmp=tmp.substr(1);
            yylval.intval=stoi(tmp);
            return REG;
}
[a-zA-Z][_a-zA-Z0-9]*   {last_string=yytext;
                         return ID;
                        }
[0-9]* {yylval.intval=atoi(yytext); return VALUE;}
[:] return SEMICOLON;
[,] return COMMA;
[$^ \t\n]+ ;

%%