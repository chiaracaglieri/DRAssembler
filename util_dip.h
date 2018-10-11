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
};

struct reg{
    int inst;
    int until;
};

extern map<int,reg> regMap;
extern vector<instruction> code;
void addReg(int,int,int);
void addInstruction(int n,string t,int i,vector<int> r);
bool isAritm(string i);