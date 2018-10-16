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
%token ID LOAD STORE MOVE
%token ADD SUB MUL
%token EQ GT LT EQ_0 LT_0 GT_0 GTE LTE GTE_0 LTE_0
%token CLEAR INCR DECR GOTO NOP
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
    |LOC VALUE  { mem_out<< "l\t" <<$2<<endl;  }
    |REGVAL VALUE VALUE { initRegister($2,$3); }
    |START VALUE    { mem_out << "s\t" << $2 << endl;
                      lc=$2-1;
                      }
    ;


instruction

    :END    { lc++;
              $$=make_node("END",-1,NULL,NULL,NULL);

            }
    |NOP { lc++; $$=make_node("NOP",-1,NULL,NULL,NULL); }
    |ar_instruction REG COMMA exp COMMA REG {   lc++;
                                                node* v1=make_node("REGISTER", $2, NULL,NULL,NULL);
                                                node* v2=make_node("REGISTER", $6, NULL,NULL,NULL);
                                                if($1==1) $$=make_node("ADD",-1,v1,$4,v2);
                                                else if($1==2) $$=make_node("SUB",-1,v1,$4,v2);
                                                else $$=make_node("MUL",-1,v1,$4,v2);
                                                }
    |rego_instruction REG   {  lc++;
                               node* v1=make_node("REGISTER", $2, NULL,NULL,NULL);
                               if($1==1) $$=make_node("CLEAR",-1,v1,NULL,NULL);
                               else if($1==2) $$=make_node("INCR",-1,v1,NULL,NULL);
                               else if($1==3) $$=make_node("DECR",-1,v1,NULL,NULL);
                            }
    |ls_instruction REG COMMA exp COMMA REG { lc++;
                                              node* v1=make_node("REGISTER",$2,NULL,NULL,NULL);
                                              node* v3=make_node("REGISTER",$6,NULL,NULL,NULL);
                                              if($1==1) $$=make_node("LOAD",-1,v1,$4,v3);
                                              else $$=make_node("STORE",-1,v1,$4,v3);
                                              }
    |cond_instruction REG COMMA REG COMMA ID {    lc++;
                                                  node* v1=make_node(last_string,-1,NULL,NULL,NULL);
                                                  node* v2=make_node("REGISTER",$2,NULL,NULL,NULL);
                                                  node* v3=make_node("REGISTER",$4,NULL,NULL,NULL);
                                                  if($1==1) $$=make_node("IF=",-1,v2,v3,v1);
                                                  else if($1==2) $$=make_node("IF<",-1,v2,v3,v1);
                                                  else if($1==3) $$=make_node("IF>",-1,v2,v3,v1);
                                                  else if($1==4) $$=make_node("IF>=",-1,v2,v3,v1);
                                                  else if($1==5) $$=make_node("IF<=",-1,v2,v3,v1);
                                                  insert_symbol(last_string,-1);
                                             }
    |cond_instruction_0 REG COMMA ID { lc++;
                                 node* v1=make_node(last_string,-1,NULL,NULL,NULL);
                                 node* v2=make_node("REGISTER",$2,NULL,NULL,NULL);
                                 if($1==1) $$=make_node("IF=0",-1,v2,v1,NULL);
                                 else if($1==2) $$=make_node("IF<0",-1,v2,v1,NULL);
                                 else if($1==3) $$=make_node("IF>0",-1,v2,v1,NULL);
                                 else if($1==4) $$=make_node("IF>=0",-1,v2,v1,NULL);
                                 else if($1==5) $$=make_node("IF<=0",-1,v2,v1,NULL);
                                 insert_symbol(last_string,-1);
                                }
    |MOVE exp COMMA REG {  lc++;
                           node* v1=make_node("REGISTER",$4,NULL,NULL,NULL);
                           $$=make_node("MOVE",-1,$2,v1,NULL);
                        }
    |ID SEMICOLON instruction { node* v1=make_node(last_string,-1,NULL,NULL,NULL);
                                $$=make_node("LABEL",-1,v1,$3,NULL);
                                insert_symbol(last_string,lc); }
    |GOTO ID {  lc++;
                insert_symbol(last_string,-1);
                node* v1=make_node(last_string,-1,NULL,NULL,NULL);
                $$=make_node("GOTO",-1,v1,NULL,NULL);
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
    |GTE  { $$=4; }
    |LTE  { $$=5; }
    ;
cond_instruction_0
    :EQ_0   { $$=1; }
    |LT_0   { $$=2; }
    |GT_0   { $$=3; }
    |GTE_0  { $$=4; }
    |LTE_0  { $$=5; }
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