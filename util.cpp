#include <unordered_map>
#include <iostream>
#include "util.h"
using namespace std;

std::string last_string;
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