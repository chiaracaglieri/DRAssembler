%{
#include <iostream>
#include <string>
#include "util.h"
int yylex(void);
inline void yyerror(const char *s) { std::cout << s << std::endl; }



%}


%union {
  int intval;
  char* str;
  struct node* nd;
}

%token <intval> VALUE
%token SEMICOLON COMMA
%token <intval> REG
%token ID LOAD STORE
%token ADD SUB MUL
%token EQ GT LT
%token CLEAR INCR
%token MEMLOC MEMLOCS LOC REGVAL
%token END START
%start program
%type <nd> instruction program
%type <intval> ls_instruction
%%

program
    :instruction    { $$=make_node("INSTRUCTION",-1,$1,NULL,NULL);}
    |instruction program    { node* i=make_node("INSTRUCTION",-1,$1,NULL,NULL);
                              node* p=make_node("PROGRAM",-1,$2,NULL,NULL);
                              $$=make_node("PROGRAM",-1,i,p,NULL);
                              appendTree($$);}
    ;

instruction
    :MEMLOC VALUE VALUE { node* v1=make_node("",$2,NULL,NULL,NULL);
                          node* v2=make_node("",$3,NULL,NULL,NULL);
                          $$=make_node("MEMLOC",-1,v1,v2,NULL);
                          lc++;
                        }
    |MEMLOCS VALUE seq  { lc++; }
    |LOC VALUE  { node* v1=make_node("",$2,NULL,NULL,NULL);
                  $$=make_node("LOC",-1,v1,NULL,NULL);
                  lc++;
                }
    |REGVAL VALUE VALUE { node* v1=make_node("",$2,NULL,NULL,NULL);
                          node* v2=make_node("",$3,NULL,NULL,NULL);
                          $$=make_node("REGVAL",-1,v1,v2,NULL);
                          lc++;
                        }
    |END    { $$=make_node("END",-1,NULL,NULL,NULL);
              lc++;
            }
    |START VALUE    { node* v1=make_node("",$2,NULL,NULL,NULL);
                      $$=make_node("START",-1,v1,NULL,NULL);
                      lc=last_value;
                    }
    |ar_instruction exp COMMA exp COMMA REG { lc++; }
    |rego_instruction REG   { lc++; }
    |ls_instruction REG COMMA REG COMMA REG {
    node* v1=make_node("REGISTER",$2,NULL,NULL,NULL);
                                              node* v2=make_node("REGISTER",$4,NULL,NULL,NULL);
                                              node* v3=make_node("REGISTER",$6,NULL,NULL,NULL);
                                                 lc++; }
    |cond_instruction exp COMMA exp COMMA label { insert_symbol(last_string,-1); lc++;}
    |label instruction { insert_symbol(last_string,lc); }
    ;
ls_instruction
    :LOAD { $$=1;}
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
