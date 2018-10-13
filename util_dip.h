#include <iostream>
#include <string>
#include <vector>
#include <deque>
#include <map>

using namespace std;

extern int counter;
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
};

extern map<int,reg> rMap;
extern map<string,int> symbolMap;
extern vector<instruction> prog;
void addReg(int,int,int);
void addInstruction(int n,string t,int i,vector<int> r,string l);
void addSymbol(string,int);
bool isAritm(string i);