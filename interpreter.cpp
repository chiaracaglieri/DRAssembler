#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <SFML/Graphics.hpp>
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
        if(b[11]==1){
            b.flip();
            return (int)(b.to_ulong()+1)*(-1);
        }
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
        cout << op << "\t" << "R" << p1 << " R" << p2 << " R" << p3 << endl;
    }
    else if(op=="ADD_I"){
        int value=0;
        if(regTable.count(p1)!=0){
            value=regTable[p1]+p2;
        }
        else if(p2!=0){
            value=p2;
        }
        regTable[p3]=value;
        cout << op << "\t" << "R" << p1 << " " << p2 << " R" << p3 << endl;
    }
    else if(op=="CLEAR"){
        regTable[p1]=0;
    }
    else if(op=="DECR"){
        regTable[p1]--;
    }
    else if(op=="EQ"){
        if(regTable[p1]==regTable[p2]){
            i+=p3-1;
            cout << op << "\t" << "R" << p1 << "R" << p2 << " " << i << endl;
        }

    }
    else if(op=="EQ_0"){
        if(regTable[p1]==0){
            i+=p2-1;
            //cout << i  << endl;
        }
    }
    else if(op=="GT"){
        if(regTable[p1]>regTable[p2]){
            i+=p3-1;
            //cout << i  << endl;
        }
    }
    else if(op=="GT_0"){
        if(regTable[p1]>0){
            i+=p2-1;
            //cout << i  << endl;
        }
    }

    else if(op=="INCR"){
        regTable[p1]++;
        //cout << regTable[p1] << endl;
    }
    else if(op=="LOAD"){
        int addr=regTable[p1]+regTable[p2];
        regTable[p3]=memTable[addr];
        cout << op << "\t" << "R" << p1 << " R" << p2 << " R" << p3 << endl;
    }
    else if(op=="LOAD_I"){
        int addr=regTable[p1]+p2;
        regTable[p3]=memTable[addr];
        cout << op << "\t" << "R" << p1 << " " << p2 << " R" << p3 << endl;
    }
    else if(op=="LT"){
        if(regTable[p1]<regTable[p2]){
            i+=p3-1;
            cout << op << "\t" << "R" << p1 << " R" << p2 << " " << i << endl;
        }

    }
    else if(op=="LT_0"){
        if(regTable[p1]<0){
            i+=p2-1;
            //cout << i  << endl;
        }
    }
    else if(op=="MUL"){
        regTable[p3]=regTable[p1]*regTable[p2];
        cout << op << "\t" << "R" << p1 << " R" << p2 << " R" << p3 << endl;
    }
    else if(op=="MUL_I"){
        regTable[p3]=regTable[p1]*p2;
        cout << op << "\t" << "R" << p1 << " " << p2 << " R" << p3 << endl;
    }
    else if(op=="STORE"){
        int addr=regTable[p1]+regTable[p2];
        memTable[addr]=regTable[p3];
        cout << op << "\t" << "R" << p1 << " R" << p2 << " R" << p3 << endl;
    }
    else if(op=="STORE_I"){
        int addr=regTable[p1]+p2;
        memTable[addr]=regTable[p3];
        cout << op << "\t" << "R" << p1 << " " << p2 << " R" << p3 << endl;
    }
    else if(op=="SUB"){
        regTable[p3]=regTable[p1]-regTable[p2];
        cout << op << "\t" << "R" << p1 << " R" << p2 << " R" << p3 << endl;
    }
    else if(op=="SUB_I"){
        int value=0;
        if(regTable.count(p1)!=0){
            value=regTable[p1]-p2;
        }
        else if(p2!=0){
            value=p2;
        }
        regTable[p3]=value;
        cout << op << "\t" << "R" << p1 << " " << p2 << " R" << p3 << endl;
    }
    else if(op=="GOTO"){
        i+=p3-1;
        cout << op << "\t" << i << endl;
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

        if(opTable[op]=="END") break;
        else if(opTable[op]=="ADD" || opTable[op]=="SUB" || opTable[op]=="MUL" || opTable[op]=="LOAD" \
                || opTable[op]=="STORE" || opTable[op]=="LT" || opTable[op]=="GT" || opTable[op]=="EQ"){
            //Retrieve parameters
            iss >> p1;
            iss >> p2;
            iss >> p3;
            execute(opTable[op],p1,p2,p3,false);
        }
        else if(opTable[op]=="LT_0" || opTable[op]=="GT_0" || opTable[op]=="EQ_0"){
            //Retrieve parameters
            iss >> p1;
            iss >> p2;
            execute(opTable[op],p1,p2,"",false);
        }
        else if(opTable[op]=="ADD_I" || opTable[op]=="SUB_I" || opTable[op]=="MUL_I" || opTable[op]=="LOAD_I" || opTable[op]=="STORE_I"){
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
        else if(opTable[op]=="CLEAR" || opTable[op]=="INCR" || opTable[op]=="DECR" || opTable[op]=="GOTO"){
            //Retrieve parameter
            iss >> p1;
            execute(opTable[op],p1,"","",false);
        }
        i++;
    }
    cout << endl;
    for (map<int,int>::iterator it=regTable.begin(); it!=regTable.end(); ++it){
        cout << "R" << it->first << " => " << it->second << endl;
    }
    cout << endl;
    for (map<int,int>::iterator it=memTable.begin(); it!=memTable.end(); ++it){
        cout << "LOC " << it->first << " => " << it->second << endl;
    }

    //sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!");


    return 0;
}