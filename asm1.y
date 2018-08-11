%{
#include <stdio.h>
#include <iostream>
#include <string>
#include "util.h"

int yylex(void);
inline void yyerror(const char *s) { std::cout << s << std::endl; }
%}


%token VALUE SEMICOLON COMMA REG
%token ID
%token LOAD STORE
%token ADD SUB MUL
%token EQ GT LT
%token CLEAR INCR
%token MEMLOC MEMLOCS LOC REGVAL
%token END START
%start program

%%

program
    :instruction
    |instruction program
    ;

instruction
    :MEMLOC VALUE VALUE { lc++; }
    |MEMLOCS VALUE seq  { lc++; }
    |LOC VALUE  { lc++; }
    |REGVAL VALUE VALUE { lc++; }
    |END    { lc++; }
    |START VALUE    { lc++; }
    |ar_instruction exp COMMA exp COMMA REG { lc++; }
    |rego_instruction REG   { lc++; }
    |ls_instruction REG COMMA REG COMMA REG { lc++; }
    |cond_instruction exp COMMA exp COMMA label { insert_symbol(last_string,-1); lc++;}
    |label instruction { insert_symbol(last_string,lc); }
    ;
ls_instruction
    :LOAD
    |STORE
    ;
ar_instruction
    :ADD
    |SUB
    |MUL
    ;
cond_instruction
    :EQ
    |LT
    |GT
    ;
rego_instruction
    :CLEAR
    |INCR
    ;
seq
    :VALUE
    |VALUE seq
    ;

label
    :ID SEMICOLON
    |ID
    ;
exp
    :REG
    |VALUE
    ;

%%
extern int yyparse();
