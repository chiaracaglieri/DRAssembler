%{
#include <iostream>
#include <string>
#include <deque>
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
%token EQ GT LT EQ_0 LT_0 GT_0
%token CLEAR INCR DECR GOTO
%token MEMLOC MEMLOCS LOC REGVAL
%token END START
%start program
%type <nd> instruction program exp seq
%type <intval> ls_instruction ar_instruction rego_instruction cond_instruction cond_instruction_0
%%

program
    :instruction    { $$=make_node("INSTRUCTION",-1,$1,NULL,NULL);}
    |state program  {}
    |instruction program    { node* i=make_node("INSTRUCTION",-1,$1,NULL,NULL);
                              node* p=make_node("PROGRAM",-1,$2,NULL,NULL);
                              $$=make_node("PROGRAM",-1,i,p,NULL);
                              append_tree($$);}
    ;

state
    :MEMLOC VALUE VALUE { initMemloc($2,$3); }
    |MEMLOCS VALUE seq  { initMemlocs($2,v); }
    |LOC VALUE  {   mem_out<< "l"<<"\t"<<$2<<endl;
                }
    |REGVAL VALUE VALUE { initRegister($2,$3); }
    |START VALUE    { mem_out<< "s"<<"\t"<<$2<<endl;
                      lc=$2-1; }
    ;


instruction

    :END    { $$=make_node("END",-1,NULL,NULL,NULL);

            }

    |ar_instruction exp COMMA exp COMMA REG {   node* v1=make_node("REGISTER", $6, NULL,NULL,NULL);
                                                if($1==1) $$=make_node("ADD",-1,$2,$4,v1);
                                                else if($1==2) $$=make_node("SUB",-1,$2,$4,v1);
                                                else $$=make_node("MUL",-1,$2,$4,v1);
                                                lc++; }
    |rego_instruction REG   {  node* v1=make_node("REGISTER", $2, NULL,NULL,NULL);
                               if($1==1) $$=make_node("CLEAR",-1,v1,NULL,NULL);
                               else if($1==2) $$=make_node("INCR",-1,v1,NULL,NULL);
                               else if($1==3) $$=make_node("DECR",-1,v1,NULL,NULL);
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
    |cond_instruction_0 exp COMMA ID { node* v1=make_node(last_string,-1,NULL,NULL,NULL);
                                 if($1==1) $$=make_node("EQ_0",-1,$2,v1,NULL);
                                 else if($1==2) $$=make_node("LT_0",-1,$2,v1,NULL);
                                 else  $$=make_node("GT_0",-1,$2,v1,NULL);
                                 insert_symbol(last_string,-1);
                                 lc++;

                                }
    |ID SEMICOLON instruction { node* v1=make_node(last_string,-1,NULL,NULL,NULL);
                                $$=make_node("LABEL",-1,v1,$3,NULL);
                                insert_symbol(last_string,lc); lc++; }
    |GOTO ID {  insert_symbol(last_string,lc);
                node* v1=make_node(last_string,-1,NULL,NULL,NULL);
                $$=make_node("GOTO",-1,v1,NULL,NULL);
                lc++;
             }
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
cond_instruction_0
    :EQ_0   { $$=1; }
    |LT_0   { $$=2; }
    |GT_0   { $$=3; }
    ;
rego_instruction
    :CLEAR  { $$=1; }
    |INCR   { $$=2; }
    |DECR   { $$=3; }
    ;
seq
    :VALUE  { v.push_front($1); }
    |VALUE seq  { v.push_front($1); }
    ;

exp
    :REG    { $$=make_node("REGISTER", $1, NULL,NULL,NULL); }
    |VALUE  { $$=make_node("VALUE", $1, NULL,NULL,NULL); }
    ;

%%