%{
#include <iostream>
#include <string>
#include <vector>
#include "util.h"
int yylex(void);
inline void yyerror(const char *s) { std::cout << s << std::endl; }

std::vector<int> v;
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
%type <nd> instruction program exp seq
%type <intval> ls_instruction ar_instruction rego_instruction cond_instruction
%%

program
    :instruction    { $$=make_node("INSTRUCTION",-1,$1,NULL,NULL);}
    |instruction program    { node* i=make_node("INSTRUCTION",-1,$1,NULL,NULL);
                              node* p=make_node("PROGRAM",-1,$2,NULL,NULL);
                              $$=make_node("PROGRAM",-1,i,p,NULL);
                              appendTree($$);}
    ;

instruction
    :MEMLOC VALUE VALUE { node* v1=make_node("VALUE",$2,NULL,NULL,NULL);
                          node* v2=make_node("VALUE",$3,NULL,NULL,NULL);
                          $$=make_node("MEMLOC",-1,v1,v2,NULL);
                          lc++;
                        }
    |MEMLOCS VALUE seq  { node* v1=make_node("VALUE",$2,NULL,NULL,NULL);
                          $$=make_node("MEMLOCS",-1,v1,$3,NULL);
                          lc++; }
    |LOC VALUE  { node* v1=make_node("VALUE",$2,NULL,NULL,NULL);
                  $$=make_node("LOC",-1,v1,NULL,NULL);
                  lc++;
                }
    |REGVAL VALUE VALUE { node* v1=make_node("VALUE",$2,NULL,NULL,NULL);
                          node* v2=make_node("VALUE",$3,NULL,NULL,NULL);
                          $$=make_node("REGVAL",-1,v1,v2,NULL);
                          lc++;
                        }
    |END    { $$=make_node("END",-1,NULL,NULL,NULL);
              lc++;
            }
    |START VALUE    { node* v1=make_node("VALUE",$2,NULL,NULL,NULL);
                      $$=make_node("START",-1,v1,NULL,NULL);
                      lc=last_value;
                    }
    |ar_instruction exp COMMA exp COMMA REG {   node* v1=make_node("REGISTER", $6, NULL,NULL,NULL);
                                                if($1==1) $$=make_node("ADD",-1,$2,$4,v1);
                                                else if($1==2) $$=make_node("SUB",-1,$2,$4,v1);
                                                else $$=make_node("MUL",-1,$2,$4,v1);
                                                lc++; }
    |rego_instruction REG   {  node* v1=make_node("REGISTER", $2, NULL,NULL,NULL);
                               if($1==1) $$=make_node("CLEAR",-1,v1,NULL,NULL);
                               else $$=make_node("INCR",-1,v1,NULL,NULL);
                               lc++; }
    |ls_instruction REG COMMA REG COMMA REG { node* v1=make_node("REGISTER",$2,NULL,NULL,NULL);
                                              node* v2=make_node("REGISTER",$4,NULL,NULL,NULL);
                                              node* v3=make_node("REGISTER",$6,NULL,NULL,NULL);
                                              if($1==1) $$=make_node("LOAD",-1,v1,v2,v3);
                                              else $$=make_node("STORE",-1,v1,v2,v3);
                                              lc++; }
    |cond_instruction exp COMMA exp COMMA ID {    node* v1=make_node(last_string,-1,NULL,NULL,NULL);
                                                  if($1==1) $$=make_node("EQ",-1,$2,$4,v1);
                                                  else if($1==2) $$=make_node("LT",-1,$2,$4,v1);
                                                  else  $$=make_node("GT",-1,$2,$4,v1);
                                                  insert_symbol(last_string,-1);
                                                  lc++;}
    |ID SEMICOLON instruction { $$=make_node(last_string,-1,$3,NULL,NULL);
                                insert_symbol(last_string,lc); }
    ;
ls_instruction
    :LOAD { $$=1; }
    |STORE  { $$=2; }
    ;
ar_instruction
    :ADD  { $$=1; }
    |SUB  { $$=2; }
    |MUL  { $$=3; }
    ;
cond_instruction
    :EQ   { $$=1; }
    |LT   { $$=2; }
    |GT   { $$=3; }
    ;
rego_instruction
    :CLEAR  { $$=1; }
    |INCR   { $$=2; }
    ;
seq
    :VALUE  { v.push_back($1);
              $$=make_seq_node(v);
              }
    |VALUE seq  { v.push_back($1); }
    ;

exp
    :REG    { $$=make_node("REGISTER", $1, NULL,NULL,NULL); }
    |VALUE  { $$=make_node("VALUE", $1, NULL,NULL,NULL); }
    ;

%%
extern int yyparse();
