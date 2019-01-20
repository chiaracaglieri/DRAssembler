/**
 * @file parser.cpp
 * @author Chiara Caglieri
 * @brief File containing main() for the assembler
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "asm.tab.h"
#include "parse_util.h"
#include <vector>

using namespace std;

extern FILE* yyin;      /**<Pointer to the file containing the data to parse*/

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

    string reg ="-regs.o";
    string fname=filename.substr(0, filename.size()-6);
    fname.append(reg);
    reg_out.open(fname);

    while (getline(tmp2,str)){
        if(str[0]=='#'){
            vector<string> v;
            istringstream buf(str);
            for(string word; buf >> word; )
                v.push_back(word);
            /*Memorize the association*/
            reg_out << "# " << v[1] <<" "<<v[2]<<endl;
            substitute("tmp.drisc", v[1],v[2]);
        }
    }
    /*Set output file*/
    string out = filename.substr(0, filename.size()-6);
    /*Set Memory and reg files*/

    string mem ="-mem.o";
    fname=filename.substr(0, filename.size()-6);
    fname.append(mem);
    mem_out.open(fname);

    out.append(".o");
    outfile.open(out);
    yyin=fopen("tmp.drisc","r");

    /*Pass 1*/
    pc=0;
    cout << "Starting Pass 1..." << endl;
    yyparse();
    cout << "Parsing finished, final pc is "<< pc << endl;
    cout << "Starting Pass 2..." << endl;

    /*Pass 2*/
    load_optable();  //Load opcodes into map structure

    node* tmp=SynTree;

    visit_tree(tmp); //Recursively visits the tree

    /*Print instructions*/
    for(int i=0; i<words.size(); i++){
        outfile << words[i].substr(0,8)<<" "<<words[i].substr(8,8)<<" "<<words[i].substr(16,8)<<" "<<words[i].substr(24,8)<<endl;
    }
    outfile.close();
    reg_out.close();
    mem_out.close();
    cout << "Pass 2 terminated, binary file created" << endl;
    remove("tmp.drisc");
    return 0;
}
