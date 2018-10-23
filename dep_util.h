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



struct instruction{
    int number;         /**<The number associated with the instruction*/
    string type;        /**<The type of the instruction*/
    int decode;         /**<The stage at which the instruction is decoded*/
    vector<int> regs;   /**<The registers needed by the instruction*/
    int imm;            /**<The immediate value, if it exists*/
    string label;       /**<The label, if it exists*/
};

struct reg{
    int inst;           /**<The instruction that is blocking the register*/
    int until;          /**<The last stage at which the register is busy*/
};

extern int counter;                 /**<Instruction counter*/
extern map<int,reg> rMap;           /**<The Register Table*/
extern map<string,int> symbolMap;   /**<The Symbol Table*/
extern vector<instruction> prog;    /**<The instructions*/

void addReg(int,int,int);
void addInstruction(int n,string t,int i,vector<int> r,string l);
void addSymbol(string,int);
bool isAritm(string i);