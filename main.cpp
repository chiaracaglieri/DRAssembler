#include <iostream>
#include <cstdio>
#include <string>
#include <fstream>
#include "asm1.tab.h"
#include "util.h"


using namespace std;

void yyparse(); //fix for MacOs

int main(int argc,  char** argv) {

    /*Pass 1*/

    lc=0;
    printf("Starting Pass 1...\n");
    yyparse();
    printf("Parsing finished, lc is %d \n Starting pass 2...\n", lc);
    /*cout << SynTree->type << SynTree->param1->type << SynTree->param2->type << SynTree->param1->param1->type
            << SynTree->param1->param1->param1->value << SynTree->param1->param1->param2->value <<
            SynTree->param2->param1->param1->param1->value <<"\n";*/

    //std::ofstream outfile ("assembled.txt");

    /*Pass 2*/
    loadOptable();
    //cout << opTable["LOAD"] << "\n";
    //Navigo l'AST e per ogni istruzione recupero il codice operativo associato
    //per ogni label controllo la tabella dei simboli e traduco ogni istruzione in binario

    //outfile << "my text here!" << std::endl;
    node* tmp=SynTree;

    VisitTree(tmp);

    outfile.close();

    return 0;
}