%{
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include "util.h"
#include "util_dip.h"

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
%type <nd> instruction program seq
%type <intval> exp ls_instruction ar_instruction rego_instruction cond_instruction cond_instruction_0
%%

program
    :instruction    { }
    |state program  { }
    |instruction program    { }
    ;

state
    :MEMLOC VALUE VALUE { }
    |MEMLOCS VALUE seq  { }
    |LOC VALUE  { }
    |REGVAL VALUE VALUE {  }
    |START VALUE    {  }
    ;


instruction

    :END    { counter++;
              addInstruction(counter,"END",vector<int>());
            }
    |NOP { counter++;
           addInstruction(lc,"NOP",vector<int>());
           }
    |ar_instruction REG COMMA exp COMMA REG {   counter++;
                                                vector<int> tmp;
                                                if($4!=-1){
                                                    tmp={$2,$4,$6};
                                                    addReg($2,$4,$6);
                                                }
                                                else{
                                                    tmp={$2,$6};
                                                    addReg($2,$6,-1);
                                                }
                                                if($1==1) addInstruction(counter,"ADD",tmp);
                                                else if($1==2) addInstruction(counter,"SUB",tmp);
                                                else addInstruction(counter,"MUL",tmp);
                                                }
    |rego_instruction REG   {  counter++;
                               vector<int> tmp={$2};
                               addReg($2,-1,-1);
                               if($1==1) addInstruction(counter,"CLEAR",tmp);
                               else if($1==2) addInstruction(counter,"INCR",tmp);
                               else if($1==3) addInstruction(counter,"DECR",tmp);
                            }
    |ls_instruction REG COMMA REG COMMA REG { counter++;
                                              vector<int> tmp={$2,$4,$6};
                                              addReg($2,$4,$6);
                                              if($1==1) addInstruction(counter,"LOAD",tmp);
                                              else addInstruction(counter,"STORE",tmp);
                                              }
    |cond_instruction REG COMMA exp COMMA ID {    counter++;
                                                  vector<int> tmp;
                                                  if($4!=-1){
                                                    tmp={$2,$4};
                                                    addReg($2,$4,-1);
                                                  }
                                                  else{
                                                    tmp={$2};
                                                    addReg($2,-1,-1);
                                                  }
                                                  addInstruction(counter,"IF",tmp);
                                             }
    |cond_instruction_0 exp COMMA ID { counter++;
                                       vector<int> tmp;
                                       if($2!=-1){
                                            tmp={$2};
                                            addReg($2,-1,-1);
                                       }
                                       else tmp={};
                                       addInstruction(counter,"IF0",tmp);
                                     }
    |MOVE exp COMMA REG {  counter++;
                           vector<int> tmp;
                           if($2!=-1){
                                tmp={$2,$4};
                                addReg($2,$4,-1);
                           }
                           else{
                                tmp={$4};
                                addReg($4,-1,-1);
                           }
                           addInstruction(counter,"MOVE",tmp);

                        }
    |ID SEMICOLON instruction {  }
    |GOTO ID {  counter++;
                addInstruction(counter,"GOTO",vector<int>());
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
    :VALUE  {  }
    |VALUE seq  { }
    ;

exp
    :REG    { $$=$1; }
    |VALUE  { $$=-1; }
    ;

%%