#include <iostream>
#include <cstdio>
#include <string>
#include "symbols.h"

using namespace std;

extern "C" {
    int yylex();
    char** yytext;
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
            case LABEL: {
                printf("Is Label! %s\n", yytext);
                break;
            }
            case START: {
                ntoken = yylex();

                printf("Updated lc value to %d\n", lc);
                break;
            }
            case END:{
                return 0;
            }
            case OPERATION: {
                //check if correspondent opcode is in optable
                break;
            }

        }
        ntoken = yylex();
    }
    return 0;
}