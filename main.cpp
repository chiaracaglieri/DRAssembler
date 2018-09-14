#include <iostream>
#include <fstream>
#include "asm1.tab.h"
#include "util.h"

using namespace std;

/*void yyparse();*/ //eliminates compile error

int main(int argc,  char** argv) {

    /*Pass 1*/
    lc=0;
    cout << "Starting Pass 1..." << std::endl;
    yyparse();
    cout << "Parsing finished, final LC is "<< lc << std::endl;
    cout << "Starting Pass 2..." << std::endl;

    /*Pass 2*/
    load_optable();  //Load opcodes into map structure

    node* tmp=SynTree;

    visit_tree(tmp); //Recursively visits the tree

    outfile.close();
    reg_out.close();
    mem_out.close();

    cout << "Pass 2 terminated, binary file created" << std::endl;

    return 0;
}
