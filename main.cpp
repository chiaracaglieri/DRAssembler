#include <iostream>
#include <cstdio>
#include <string>
#include <unordered_map>
#include "symbols.h"
#include "util.h"

using namespace std;

extern "C"
{
    int yyparse(void);
    int yylex(void);


}

/*bool searchOP(char* opname){
    for(int i=0; i< optable.size(); i++){
        if(opname==optable[i].literal) return true;
    }
    return false;
}

*/
int main() {
    lc=0;

    yyparse();

    /*int ntoken, vtoken;

    //Read new token
    ntoken = yylex();
    while(ntoken){
        // printf("%d %s\n", ntoken, yytext);
        switch(ntoken){
            case LABEL: {
                if (symTable.find(yytext)!=symTable.end())
                    printf("Label already in symtab...\n");
                else{
                    symTable[yytext]=lc;
                    printf("Inserted label %s with lc %d\n", yytext, symTable[yytext]);
                }
                break;
            }
            case START: {
                ntoken = yylex();
                lc=strtol(yytext,NULL,0);
                printf("Updated lc value to %d\n", lc);
                break;
            }
            case I_END: {
                lc++;
                break;
            }
            case END:{
                return 0;
            }
            case OPERATION: {
                if(searchOP(yytext)) break;
                else printf("Error! Opcode not in table\n");
                break;
            }

        }
        ntoken = yylex();
    }*/
    return 0;
}