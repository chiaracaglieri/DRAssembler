%{
#include <stdio.h>
#include <stdlib.h>
#include "symbols.h"

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
    :MEMLOC VALUE VALUE { lc++; printf("Updated lc to %d\n", lc);}
    |MEMLOCS VALUE seq  { lc++; printf("Updated lc to %d\n", lc);}
    |LOC VALUE  { lc++; printf("Updated lc to %d\n", lc);}
    |REGVAL VALUE VALUE { lc++; printf("Updated lc to %d\n", lc);}
    |END
    |START VALUE    { lc=last_value; printf("Updated lc to %d\n", lc);}
    |ar_instruction exp COMMA exp COMMA REG { lc++; printf("Updated lc to %d\n", lc);}
    |rego_instruction REG   { lc++; printf("Updated lc to %d\n", lc);}
    |ls_instruction REG COMMA REG COMMA REG { lc++; printf("Updated lc to %d\n", lc);}
    |cond_instruction exp COMMA exp COMMA label { lc++; printf("Updated lc to %d\n", lc);}
    |label instruction  { lc++; printf("Updated lc to %d\n", lc);}
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
    :ID SEMICOLON   { checkSymbol(last_string, lc); }
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
