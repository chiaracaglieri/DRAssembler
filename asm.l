%{
/**
 * @file asm.l
 * @author Chiara Caglieri
 * @brief Flex token file
 */

#include <string>
#include <iostream>
#include "parse_util.h"
#include "asm.tab.h"

string tmp;     /**<Temporary buffer to store data*/
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
"mod" return MOD;
"div" return DIV;
"load" return LOAD;
"store" return STORE;
"move" return MOVE;
"nop" return NOP;
"if=0" return EQ_0;
"if>0" return GT_0;
"if<0" return LT_0;
"if>=0" return GTE_0;
"if<=0" return LTE_0;
"if=" return EQ;
"if>" return GT;
"if<" return LT;
"if>=" return GTE;
"if<=" return LTE;
"if!=" return NEQ;
"if!=0" return NEQ_0;
"goto" return GOTO;
"call" return CALL;
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
"#define R"[a-zA-Z][_a-zA-Z0-9]*" R"[0-9]* ;
[$^ \t\n]+ ;

%%
