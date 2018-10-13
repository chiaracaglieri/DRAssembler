%{
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <deque>
#include "util_dip.h"
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
%type <nd> instruction program seq
%type <intval> exp ls_instruction ar_instruction rego_instruction cond_instruction cond_instruction_0
%%

program
    :instruction    { }
    |state program  { }
    |instruction program    { }
    ;

state
    :MEMLOC VALUE VALUE {  }
    |MEMLOCS VALUE seq  {  }
    |LOC VALUE  {  }
    |REGVAL VALUE VALUE { }
    |START VALUE    { }
    ;


instruction

    :END    { counter++;
              addInstruction(counter,"END",-1,vector<int>(),"");
            }
    |NOP { counter++;
           addInstruction(counter,"NOP",-1,vector<int>(),"");
           }
    |ar_instruction REG COMMA REG COMMA REG {   counter++;
                                                vector<int> tmp;

                                                tmp={$2,$4,$6};
                                                addReg($2,$4,$6);

                                                if($1==1) addInstruction(counter,"ADD",-1,tmp,"");
                                                else if($1==2) addInstruction(counter,"SUB",-1,tmp,"");
                                                else addInstruction(counter,"MUL",-1,tmp,"");
                                                }
    |ar_instruction REG COMMA VALUE COMMA REG {   counter++;
                                                vector<int> tmp;

                                                 tmp={$2,$6};
                                                 addReg($2,$6,-1);

                                                 if($1==1) addInstruction(counter,"ADD_I",$4,tmp,"");
                                                 else if($1==2) addInstruction(counter,"SUB_I",$4,tmp,"");
                                                 else addInstruction(counter,"MUL_I",$4,tmp,"");
                                                }
    |rego_instruction REG   {  counter++;
                               vector<int> tmp={$2};
                               addReg($2,-1,-1);
                               if($1==1) addInstruction(counter,"CLEAR",-1,tmp,"");
                               else if($1==2) addInstruction(counter,"INCR",-1,tmp,"");
                               else if($1==3) addInstruction(counter,"DECR",-1,tmp,"");
                            }
    |ls_instruction REG COMMA REG COMMA REG { counter++;
                                              vector<int> tmp={$2,$4,$6};
                                              addReg($2,$4,$6);
                                              if($1==1) addInstruction(counter,"LOAD",-1,tmp,"");
                                              else addInstruction(counter,"STORE",-1,tmp,"");
                                              }
    |ls_instruction REG COMMA VALUE COMMA REG { counter++;
                                                vector<int> tmp={$2,$6};
                                                addReg($2,$6,-1);
                                                if($1==1) addInstruction(counter,"LOAD_I",$4,tmp,"");
                                                else addInstruction(counter,"STORE_I",$4,tmp,"");
                                              }
    |cond_instruction REG COMMA REG COMMA ID {    counter++;
                                                  vector<int> tmp;
                                                  tmp={$2,$4};
                                                  addReg($2,$4,-1);

                                                  if($1==1) addInstruction(counter,"EQ",-1,tmp,last_string);
                                                  else if($1==2) addInstruction(counter,"LT",-1,tmp,last_string);
                                                  else if($1==3) addInstruction(counter,"GT",-1,tmp,last_string);
                                                  else if($1==4) addInstruction(counter,"GTE",-1,tmp,last_string);
                                                  else if($1==5) addInstruction(counter,"LTE",-1,tmp,last_string);

                                                  addSymbol(last_string,-1);
                                             }
    |cond_instruction_0 exp COMMA ID { counter++;
                                       vector<int> tmp;
                                       if($2!=-1){
                                            tmp={$2};
                                            addReg($2,-1,-1);
                                       }
                                       else tmp={};
                                       if($1==1) addInstruction(counter,"EQ_0",-1,tmp,last_string);
                                       else if($1==2) addInstruction(counter,"LT_0",-1,tmp,last_string);
                                       else if($1==3) addInstruction(counter,"GT_0",-1,tmp,last_string);
                                       else if($1==4) addInstruction(counter,"GTE_0",-1,tmp,last_string);
                                       else if($1==5) addInstruction(counter,"LTE_0",-1,tmp,last_string);
                                       addSymbol(last_string,-1);
                                     }
    |MOVE REG COMMA REG {  counter++;
                           vector<int> tmp;

                           tmp={$2,$4};
                           addReg($2,$4,-1);
                           addInstruction(counter,"MOVE",-1,tmp,"");

                        }
    |MOVE VALUE COMMA REG {  counter++;
                             vector<int> tmp;

                             tmp={$4};
                             addReg($4,-1,-1);
                             addInstruction(counter,"MOVE_I",$2,tmp,"");
                        }
    |ID SEMICOLON instruction { addSymbol(last_string,counter); }
    |GOTO ID {  counter++;
                addSymbol(last_string,-1);
                addInstruction(counter,"GOTO",-1,vector<int>(),last_string);
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
    :VALUE  { v1.push_front($1); }
    |VALUE seq  { v1.push_front($1); }
    ;

exp
    :REG    { $$=$1; }
    |VALUE  { $$=-1; }
    ;

%%