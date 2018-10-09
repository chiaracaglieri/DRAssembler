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
    int value;
};

extern map<int,reg> regMap;
extern map<int,int> memMap;
extern vector<instruction> code;
void initMemlocs1(int,deque<int>);
void initReg(int,int);
void addReg(int,int,int);
void addInstruction(int n,string t,int i,vector<int> r);
bool isAritm(string i);