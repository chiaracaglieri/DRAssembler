#include <iostream>
#include <string>
#include <vector>
#include <map>

using namespace std;

extern int counter;

struct instruction{
    int number;
    string type;
    int fetch;
    vector<int> regs;
};

struct reg{
    int inst;
    int until;
};

extern map<int,reg> regs;
extern vector<instruction> code;
void addReg(int,int,int);
void addInstruction(int n,string t,vector<int> r);