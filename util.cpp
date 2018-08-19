#include <unordered_map>
#include <iostream>
#include <fstream>
#include <string>
#include "util.h"
using namespace std;

std::string last_string;
std::string tmp;
int last_value;
std::unordered_map<std::string, int> symTable;
std::unordered_map<std::string, std::string > opTable;

int lc;

bool find_symbol(std::string sym){
    if(symTable.find(sym)!=symTable.end())
        return true;
    else return false;
}

void insert_symbol(std::string sym, int lc){

    if(find_symbol(sym)) cout << "Symbol already" << "\n";
    else {symTable[sym]=lc; cout << "Symbol " << sym << " inserted" << "\n";}
}

/*AST Function Implementation*/

tree SynTree=NULL;

tree make_node(std::string type, int value, node* p1, node* p2, node* p3){
    tree new_node= new node;

    new_node->lc=lc;
    new_node->value=value;
    new_node->type=type;
    new_node->param1=p1;
    new_node->param2=p2;
    new_node->param3=p3;

    return new_node;
}

tree make_seq_node(std::vector<int> seq){
    tree new_node= new node;

    new_node->lc=lc;
    new_node->value=-1;
    new_node->seq=seq;
    new_node->type="SEQ";
    new_node->param1=NULL;
    new_node->param2=NULL;
    new_node->param3=NULL;

    return new_node;
}

void appendTree(node* node) {
    SynTree = node;
}

void loadOptable(){
    ifstream opcodes ("optable.txt");

    std::string word;
    while(opcodes >> word){
        std::string op=word;
        opcodes >> word;
        opTable[op]=word;
    }
}