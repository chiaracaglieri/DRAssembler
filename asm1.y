%{
#include <cstdio>
#include <iostream>
using namespace std;


extern int yylex();
extern "C" int yyparse();

void yyerror(const char *s);

%}

%token VALUE
%token SEMICOLON COMMA REG
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

void yyerror(const char *s) {
	cout << "EEK, parse error!  Message: " << s << endl;
	// might as well halt now:
	exit(-1);
}