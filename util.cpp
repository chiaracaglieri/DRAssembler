#include <unordered_map>
#include <iostream>
#include <string>
#include "util.h"
using namespace std;

std::string last_string;
std::string tmp;
int last_value;
std::unordered_map<std::string, int> symTable;
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

void appendTree(node* node) {
    SynTree = node;
}