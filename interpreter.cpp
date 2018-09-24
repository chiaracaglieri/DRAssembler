#include <iostream>
#include <fstream>
#include <sstream>
#include "state.h"

void execute(string op, string a, string b, string c, bool p4){
    unsigned long p1,p2,p3;
    bitset<6> tmp1(a);
    p1=tmp1.to_ulong();
    if(p4==true){   //b is 12 bits
        bitset<12> tmp(b);
        p2=tmp.to_ulong();
    }
    else{
        bitset<6> tmp2(b);
        p2=tmp2.to_ulong();
    }
    bitset<6> tmp3(c);
    p3=tmp3.to_ulong();

    if(op=="ADD_I"){
            regTable[p3]=regTable[p1]+p2;
            cout << regTable[p3] << endl;
    }

}

int main(int argc,  char** argv) {

    loadOptable();
    loadRegisters();
    loadMemory();
    cout << start << endl;
    cout << loc << endl;
    loadProgram();

    int terminate=0;
    int i=loc;
    string op;
    string p1;
    string p2;
    string p3;
    string p4;
    bool flag;

    while (true) {
        istringstream iss(memTable[i]);
        iss >> op;
        cout << op << " " << opTable[op] << endl;
        if(opTable[op]=="END") break;
        else if(opTable[op]=="ADD_I"){
            //Retrieve parameters
            iss >> p1;
            iss >> p2;
            iss >> p3;
            if(memTable[i].length()>29){ //Param p4 exists
                iss >> p4;
                stringstream ss;
                ss << p2 << p4;
                p2 = ss.str();
                flag=true;
            }
        }
        execute(opTable[op],p1,p2,p3,flag);
        i++;
    }
    return 0;
}