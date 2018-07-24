#include <iostream>
#include <cstdio>
#include <string>
#include "symbols.h"
#include "util.h"

using namespace std;

extern "C" {
    int yylex();
    char* yytext;
}

bool searchOP(char* opname){
    for(int i=0; i< optable.size(); i++){
        if(opname==optable[i].literal) return true;
    }
    return false;
}
int lc; //location counter

int main() {
    lc=0;
    int ntoken, vtoken;
    //Read new token
    ntoken = yylex();
    while(ntoken){
        printf("%d %s\n", ntoken, yytext);
        switch(ntoken){
            case I_OP_1: {

            } //Operation expecting 1 Integer parameter
            case I_OP_2:{

            }//Operation expecting 2 Integer parameters
            case LABEL: {
                printf("Is Label! %s\n", yytext);
                break;
            }
            case START: {
                ntoken = yylex();
                lc=strtol(yytext,NULL,0);
                printf("Updated lc value to %d\n", lc);
                break;
            }
            case END:{
                return 0;
            }
           /* case OPERATION: {
                if(searchOP(yytext)) break;
                else printf("Error! Opcode not in table\n");
                break;
            }*/

        }
        ntoken = yylex();
    }
    return 0;
}