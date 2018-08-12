%{
#include <iostream>
#include <string>
#include "util.h"
int yylex(void);
inline void yyerror(const char *s) { std::cout << s << std::endl; }



%}


%union {
  int intval;
  struct node* nd;
}

%token <intval> VALUE
%token SEMICOLON COMMA REG
%token ID
%token LOAD STORE
%token ADD SUB MUL
%token EQ GT LT
%token CLEAR INCR
%token MEMLOC MEMLOCS LOC REGVAL
%token END START
%start program
%type <nd> instruction program
%%

program
    :instruction    { $$=make_node("INSTRUCTION",-1,$1,NULL,NULL);}
    |instruction program    { $$=make_node("PROGRAM",-1,$1,$2,NULL); appendTree($$);}
    ;

instruction
    :MEMLOC VALUE VALUE { node* v1=make_node("",$2,NULL,NULL,NULL);
                          node* v2=make_node("",$3,NULL,NULL,NULL);
                          $$=make_node("MEMLOC",-1,v1,v2,NULL);
                          lc++;
                        }
    |MEMLOCS VALUE seq  { lc++; }
    |LOC VALUE  { lc++; }
    |REGVAL VALUE VALUE { lc++; }
    |END    { lc++; }
    |START VALUE    { lc=last_value; }
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
