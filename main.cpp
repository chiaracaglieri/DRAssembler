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
    /*cout << SynTree->type << SynTree->param1->type << SynTree->param2->type << SynTree->param1->param1->type
            << SynTree->param1->param1->param1->value << SynTree->param1->param1->param2->value <<
            SynTree->param2->param1->param1->param1->value <<"\n";*/

    std::ofstream outfile ("assembled.txt");

    //outfile << "my text here!" << std::endl;

    outfile.close();

    return 0;
}