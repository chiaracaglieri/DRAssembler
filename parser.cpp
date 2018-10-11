#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "asm1.tab.h"
#include "util.h"
#include <vector>

using namespace std;

#if defined(__APPLE__) && defined(__MACH__)
void yyparse();     //eliminates MacOS compile error
#endif

extern FILE* yyin;

int main(int argc,  char** argv) {

    /*Preprocessing*/
    cout << "Start preprocessing..." << endl;
    ifstream file(argv[1]);
    /*Check file type*/
    string filename=argv[1];
    if(filename.substr(filename.find_last_of(".") + 1) != "drisc") {
        cerr << "Error,  \""<< filename << "\" is not of type .drisc..." << endl;
        exit(EXIT_FAILURE);
    }
    ofstream tmp1("tmp.drisc");

    tmp1 << file.rdbuf();
    tmp1.close();

    ifstream tmp2("tmp.drisc");
    string str;
    while (getline(tmp2,str)){
        if(str[0]=='#'){
            vector<string> v;
            istringstream buf(str);
            for(string word; buf >> word; )
                v.push_back(word);
            substitute("tmp.drisc", v[1],v[2]);
        }
    }
    /*Set output file*/
    string out = filename.substr(0, filename.size()-6);
    out.append(".o");
    outfile.open(out);
    yyin=fopen("tmp.drisc","r");

    /*Pass 1*/
    lc=0;
    cout << "Starting Pass 1..." << endl;
    yyparse();
    cout << "Parsing finished, final LC is "<< lc << endl;
    cout << "Starting Pass 2..." << endl;

    /*Pass 2*/
    load_optable();  //Load opcodes into map structure

    node* tmp=SynTree;

    visit_tree(tmp); //Recursively visits the tree

    outfile.close();
    reg_out.close();
    mem_out.close();
    cout << "Pass 2 terminated, binary file created" << endl;
    remove("tmp.drisc");
    return 0;
}
