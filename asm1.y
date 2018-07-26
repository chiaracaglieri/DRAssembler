%{
#include <stdio.h>
#include <stdlib.h>

int yylex();
void yyerror();
%}

%token VALUE SEMICOLON COMMA REG
%token ID
%token LOAD STORE
%token ADD SUB MUL
%token EQ GT LT
%token CLEAR INCR END START
%token MEMLOC MEMLOCS LOC REGVAL


%%

program
    :instruction
    |pseudo_instruction
    |label
    ;
instruction
    :MEMLOC VALUE VALUE
    |MEMLOCS VALUE seq
    |LOC VALUE
    |REGVAL VALUE VALUE
    |ar_instruction exp COMMA exp COMMA REG
    |rego_instruction REG
    |ls_instruction REG COMMA REG COMMA REG
    |cond_instruction exp COMMA exp COMMA label
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
pseudo_instruction
    :END
    |START VALUE
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
void yyerror()
{
  printf("\nError");
}

int main(){

    yyparse();
    return 0;
}