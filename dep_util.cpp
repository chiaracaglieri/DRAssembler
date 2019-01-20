/**
 * @file dep_util.cpp
 * @author Chiara Caglieri
 * @brief File containing the implementation of
 *        utility functions for the analyzer
 */

#include <iostream>
#include <string>
#include <vector>
#include <deque>
#include <sstream>
#include <map>
#include "dep_util.h"

using namespace std;

int counter=0;
map<int,reg> rMap;
map<string,int> symbolMap;
map<int,string> symRegTable;
vector<instruction> prog;

/**
 * @brief Adds given symbol to Symbol Table
 * @param s the symbol
 * @param pos the position at which the symbol is
 *            found in the code
 */
void addSymbol(string s, int pos){
    if(!symbolMap[s] || symbolMap[s]==-1) symbolMap[s]=pos;
}

/**
 * @brief Adds registers to Register Table
 * @param r1 the first register
 * @param r2 the second register
 * paramm r3 the third register
 */
void addReg(int r1, int r2, int r3){
    if(rMap.find(r1)==rMap.end()){
        reg a;
        a.inst=-1;
        a.until=-1;
        rMap[r1]=a;
    }

    if(r2!=-1 && rMap.find(r2)==rMap.end()){
        reg b;
        b.inst=-1;
        b.until=-1;
        rMap[r2]=b;
    }
    if(r3!=-1 && rMap.find(r3)==rMap.end()){
        reg c;
        c.inst=-1;
        c.until=-1;
        rMap[r3]=c;
    }

}

/**
 * @brief Formats and prints the given instruction
 * @param i the instruction to be printed
 */
void printInstruction(instruction i){
    cout << i.number << ": ";
    if(i.type=="LOAD_I") cout <<"LOAD ";
    else if(i.type=="STORE_I") cout <<"STORE ";
    else if(i.type=="ADD_I") cout <<"ADD ";
    else if(i.type=="SUB_I") cout <<"SUB ";
    else if(i.type=="MUL_I") cout <<"MUL ";
    else if(i.type=="DIV_I") cout <<"DIV ";
    else if(i.type=="MOD_I") cout <<"MOD ";
    else if(i.type=="MOVE_I"){
        cout <<"MOVE "<<"#"<<i.imm<<" "<<checkSym(i.regs[0])<<endl;
        return;
    }
    else{
        if(i.type=="EQ") cout << "IF= ";
        else if(i.type=="LT") cout << "IF< ";
        else if(i.type=="GT") cout << "IF> ";
        else if(i.type=="LTE") cout << "IF<= ";
        else if(i.type=="GTE") cout << "IF>= ";
        else if(i.type=="NEQ") cout << "IF!= ";
        else if(i.type=="EQ_0") cout << "IF=0 ";
        else if(i.type=="LT_0") cout << "IF<0 ";
        else if(i.type=="GT_0") cout << "IF>0 ";
        else if(i.type=="LTE_0") cout << "IF<=0 ";
        else if(i.type=="GTE_0") cout << "IF>=0 ";
        else if(i.type=="NEQ_0") cout << "IF!=0 ";
        else cout<<i.type<<" ";
        for(int x:i.regs){
            cout<<checkSym(x)<<" ";
        }
        cout << i.label<<endl;
        return;
    }
    cout<<checkSym(i.regs[0])<<" #"<<i.imm<<" "<<checkSym(i.regs[1])<<" "<<i.label<<endl;
}
/**
 * @brief Initializes an instruction object
 * @param n index
 * @param t type
 * @param imm immediate value
 * @param r registers used
 * @param l label
 */
void addInstruction(int n,string t, int imm, vector<int> r,string l){
    instruction i;
    i.number=n;
    i.type=t;
    i.regs=r;
    i.decode=n;
    i.label=l;
    if(t=="ADD_I" || t=="SUB_I" || t=="MUL_I" || t=="LOAD_I" || t=="STORE_I" || t=="MOVE_I" || t=="DIV_I" || t=="MOD_I")
        i.imm=imm;
    prog.push_back(i);
    printInstruction(i);
}

/**
 * @brief Checks if the given instruction is of arithmetic type
 * @param i the instruction
 * @return true if the instruction is of arithmetic type, false otherwise
 */
bool isAritm(string i){
    if(i=="ADD" || i=="SUB" || i=="ADD_I" || i=="SUB_I" || i=="MUL" || i=="MUL_I" || i=="INCR" \
        || i=="DECR" || i=="CLEAR" || i=="MOVE" || i=="MOVE_I" || i=="DIV_I" || i=="DIV" || i=="MOD_I" || i=="MOD") return true;
    else return false;
}

string checkSym(int reg){
    if(symRegTable.find(reg) == symRegTable.end() ) {
        stringstream s;
        s << "R"<< reg;
        return s.str();
    }
    else return symRegTable[reg];
}