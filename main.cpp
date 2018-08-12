#include <iostream>
#include <cstdio>
#include <string>
#include <fstream>
#include "asm1.tab.h"
#include "util.h"


using namespace std;


int main(int argc,  char** argv) {
    lc=0;
    printf("Starting Pass 1...\n");
    yyparse();
    printf("Parsing finished, lc is %d \n Starting pass 2...\n", lc);
    cout << SynTree->type << SynTree->param1->type << SynTree->param2->type<< SynTree->param2->param1->type <<"\n";
    return 0;
}