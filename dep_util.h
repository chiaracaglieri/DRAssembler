/**
 * @file dep_util.h
 * @author Chiara Caglieri
 * @brief Header containing utility functions for the analyzer
 */

#include <iostream>
#include <string>
#include <vector>
#include <deque>
#include <map>

using namespace std;

extern int counter;
extern int loc;
extern deque<int> v1;                            //Contains the values for memlocs

struct instruction{
    int number;
    string type;
    int decode;
    vector<int> regs;
    int imm;
    string label;
};

struct reg{
    int inst;
    int until;
    int value;
};

extern map<int,reg> rMap;
extern map<string,int> symbolMap;
extern vector<instruction> prog;
void addReg(int,int,int);
void addInstruction(int n,string t,int i,vector<int> r,string l);
void addSymbol(string,int);
bool isAritm(string i);