#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "util_dip.h"

using namespace std;

int counter=0;
map<int,reg> regs;
vector<instruction> code;

void addReg(int r1, int r2, int r3){
    reg a;
    a.inst=-1;
    a.until=-1;
    regs[r1]=a;
    if(r2!=-1){
        reg b;
        b.inst=-1;
        b.until=-1;
        regs[r2]=b;
    }
    if(r3!=-1){
        reg c;
        c.inst=-1;
        c.until=-1;
        regs[r3]=c;
    }


}
void addInstruction(int n,string t,vector<int> r){
    instruction i;
    i.number=n;
    i.type=t;
    i.regs=r;
    i.fetch=n;
    code.push_back(i);

    cout<<"I added:\t"<<i.number<<". "<<i.type<<"That operates on\t";
    for(int x: i.regs){
        cout<<x<<" ";
    }
    cout<< endl;
}