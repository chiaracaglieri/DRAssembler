#include <iostream>
#include <string>
#include <vector>
#include <deque>
#include <map>
#include "util_dip.h"

using namespace std;

int counter=0;
deque<int> v1;                           //Contains the values for memlocs
map<int,reg> regMap;
map<int,int> memMap;
vector<instruction> code;

void initMemlocs1(int l, deque<int> v){
    for(int i=0; i<v.size(); i++){
        memMap[l]=v[i];
        l++;
    }
}

void initReg(int r, int v){
    reg a;
    a.inst=-1;
    a.until=-1;
    a.value=v;
    regMap[r]=a;
}

void addReg(int r1, int r2, int r3){
    if(regMap.find(r1)==regMap.end()){
        reg a;
        a.inst=-1;
        a.until=-1;
        a.value=0;
        regMap[r1]=a;
    }

    if(r2!=-1 && regMap.find(r2)==regMap.end()){
        reg b;
        b.inst=-1;
        b.until=-1;
        b.value=0;
        regMap[r2]=b;
    }
    if(r3!=-1 && regMap.find(r3)==regMap.end()){
        reg c;
        c.inst=-1;
        c.until=-1;
        c.value=0;
        regMap[r3]=c;
    }

}
void addInstruction(int n,string t, int imm, vector<int> r){
    instruction i;
    i.number=n;
    i.type=t;
    i.regs=r;
    i.fetch=n;
    if(t=="ADD_I" || t=="SUB_I" || t=="MUL_I" || t=="LOAD_I" || t=="STORE_I" || t=="MOVE_I")
        i.imm=imm;
    code.push_back(i);

    cout<<"I added:\t"<<i.number<<". "<<i.type<<"That operates on\t";
    for(int x: i.regs){
        cout<<x<<" ";
    }
    cout<< endl;
}