#include <iostream>
#include <string>
#include <vector>
#include <deque>
#include <map>
#include "util_dip.h"

using namespace std;

int counter=0;
deque<int> v1;                           //Contains the values for memlocs
map<int,reg> rMap;
map<string,int> symbolMap;
vector<instruction> prog;

void addSymbol(string s, int pos){
    symbolMap[s]=pos;
}

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

void printInstruction(instruction i){
    cout << i.number << ": ";
    if(i.type=="LOAD_I") cout <<"LOAD ";
    else if(i.type=="STORE_I") cout <<"STORE ";
    else if(i.type=="ADD_I") cout <<"ADD ";
    else if(i.type=="SUB_I") cout <<"SUB ";
    else if(i.type=="MUL_I") cout <<"MUL ";
    else if(i.type=="MOVE_I"){
        cout <<"MOVE "<<"#"<<i.imm<<" R"<<i.regs[0]<<endl;
        return;
    }
    else{
        if(i.type=="EQ") cout << "IF= ";
        else if(i.type=="LT") cout << "IF< ";
        else if(i.type=="GT") cout << "IF> ";
        else if(i.type=="LTE") cout << "IF<= ";
        else if(i.type=="GTE") cout << "IF>= ";
        else if(i.type=="EQ_0") cout << "IF=0 ";
        else if(i.type=="LT_0") cout << "IF<0 ";
        else if(i.type=="GT_0") cout << "IF>0 ";
        else if(i.type=="LTE_0") cout << "IF<=0 ";
        else if(i.type=="GTE_0") cout << "IF>=0 ";
        else cout<<i.type<<" ";
        for(int x:i.regs){
            cout<<"R"<<x<<" ";
        }
        cout << i.label<<endl;
        return;
    }
    cout<<"R"<<i.regs[0]<<" #"<<i.imm<<" R"<<i.regs[1]<<" "<<i.label<<endl;
}

void addInstruction(int n,string t, int imm, vector<int> r,string l){
    instruction i;
    i.number=n;
    i.type=t;
    i.regs=r;
    i.decode=n;
    i.label=l;
    if(t=="ADD_I" || t=="SUB_I" || t=="MUL_I" || t=="LOAD_I" || t=="STORE_I" || t=="MOVE_I")
        i.imm=imm;
    prog.push_back(i);
    printInstruction(i);
}

bool isAritm(string i){
    if(i=="ADD" || i=="SUB" || i=="ADD_I" || i=="SUB_I" || i=="MUL" || i=="MUL_I" || i=="INCR" \
        || i=="DECR" || i=="CLEAR" || i=="MOVE" || i=="MOVE_I") return true;
    else return false;
}

