%{
#include <stdio.h>
#include <stdlib.h>
#include "asm1.tab.h"

/* Flex functions */
extern int yylex(void);
extern void yyterminate();
void yyerror(const char *s);

int yydebug=1;

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
    |instruction instructions
    ;
instructions
    :instruction
    |instruction instructions
    ;
instruction
    :MEMLOC VALUE VALUE
    |MEMLOCS VALUE seq
    |LOC VALUE
    |REGVAL VALUE VALUE
    |END
    |START VALUE
    |ar_instruction exp COMMA exp COMMA REG
    |rego_instruction REG
    |ls_instruction REG COMMA REG COMMA REG
    |cond_instruction exp COMMA exp COMMA label
    |label instruction
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

/* Display error messages */
void yyerror(const char *s)
{
	printf("ERROR: %s\n", s);
}
