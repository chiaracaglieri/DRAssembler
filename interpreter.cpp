#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "state.h"

using namespace std;

int i;

int get_int(string s, int nbits){
    if(nbits==6){
        bitset<6> b(s);
        if(b[5]==1){
            b.flip();
            return (int)(b.to_ulong()+1)*(-1);
        }
        return (int)(b.to_ulong());
    }
    if(nbits==12){
        bitset<12> b(s);
        return (int)(b.to_ulong());
    }
    else{
        cout << "Error" << endl;
        exit(EXIT_FAILURE);
    }
}

void execute(string op, string a, string b, string c, bool p4){
    int p1,p2,p3;
    p1=get_int(a,6);
    if(p4==true) p2=get_int(b,12);
    else if(!b.empty()) p2=get_int(b,6);
    if(!c.empty()) p3=get_int(c,6);
    if(op=="ADD"){
        regTable[p3]=regTable[p1]+regTable[p2];
        cout << regTable[p3] << endl;
    }
    if(op=="ADD_I"){
        int value=0;
        if(regTable.count(p1)!=0){
            value=regTable[p1]+p2;
        }
        else if(p2!=0){
            value=p2;
        }
        regTable[p3]=value;
    }
    else if(op=="CLEAR"){
        regTable[p1]=0;
    }
    else if(op=="INCR"){
        regTable[p1]++;
        cout << regTable[p1] << endl;
    }
    else if(op=="LOAD"){
        int addr=regTable[p1]+regTable[p2];
        regTable[p3]=memTable[addr];
        cout << regTable[p3] << endl;
    }
    else if(op=="LT"){
        if(regTable[p1]<regTable[p2]){
            i+=p3-1;
            cout << i  << endl;
        }
    }
    else if(op=="MUL"){
        regTable[p3]=regTable[p1]*regTable[p2];
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
    i=loc;
    string op;
    string p1;
    string p2;
    string p3;
    string p4;

    while (true) {
        istringstream iss(memCode[i]);
        iss >> op;
        cout << op << " " << opTable[op] << endl;
        if(opTable[op]=="END") break;
        else if(opTable[op]=="ADD"){
            //Retrieve parameters
            iss >> p1;
            iss >> p2;
            iss >> p3;
            execute(opTable[op],p1,p2,p3,false);
        }
        else if(opTable[op]=="ADD_I"){
            //Retrieve parameters
            iss >> p1;
            iss >> p2;
            iss >> p3;
            if(memCode[i].length()>29){ //Param p4 exists
                iss >> p4;
                stringstream ss;
                ss << p2 << p4;
                p2 = ss.str();
                execute(opTable[op],p1,p2,p3,true);
            }
            else execute(opTable[op],p1,p2,p3,false);
        }
        else if(opTable[op]=="CLEAR"){
            //Retrieve parameter
            iss >> p1;
            execute(opTable[op],p1,"","",false);
        }
        else if(opTable[op]=="INCR"){
            //Retrieve parameter
            iss >> p1;
            execute(opTable[op],p1,"","",false);
        }
        else if(opTable[op]=="LOAD"){
            //Retrieve parameters
            iss >> p1;
            iss >> p2;
            iss >> p3;
            execute(opTable[op],p1,p2,p3,false);
        }
        else if(opTable[op]=="LT"){
            //Retrieve parameters
            iss >> p1;
            iss >> p2;
            iss >> p3;
            execute(opTable[op],p1,p2,p3,false);
        }
        else if(opTable[op]=="MUL"){
            //Retrieve parameters
            iss >> p1;
            iss >> p2;
            iss >> p3;
            execute(opTable[op],p1,p2,p3,false);
        }
        i++;
    }
    return 0;
}