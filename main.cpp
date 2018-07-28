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
    printf("Starting Pass 1\n");
    yyparse();
    lc++; //to include END statement

    printf("Parsing finished, value of lc is %d\n", lc);
    printf("Starting Pass 2\n");

    return 0;
}