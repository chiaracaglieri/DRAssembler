#include <iostream>
#include <cstdio>
#include <string>
#include <fstream>
#include "util.h"

using namespace std;

extern "C"
{
    int yyparse(void);
    int yylex(void);
    char* yytext;
    void yyrestart();


}

/*bool searchOP(char* opname){
    for(int i=0; i< optable.size(); i++){
        if(opname==optable[i].literal) return true;
    }
    return false;
}

*/
int main(int argc,  char** argv) {
    lc=0;
    printf("Starting Pass 1\n");
    yyparse();
    lc++; //to include END statement

    printf("Parsing finished, value of lc is %d\n", lc);
    printf("Starting Pass 2\n");

    std::ofstream outfile ("final.txt");

   //outfile << "my text here!" << std::endl;

    outfile.close();
    return 0;
}