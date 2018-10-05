#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "state.h"

using namespace std;

int i;
bool disableManagement=false;
int counter=-1;

/** \function get_int
  * \brief generates the int corresponding to a string of binary digits
  * \param s the string
  * \param nbits the number of bits in the string
  * \return (int) b.to_ulong() the integer translation of the digits
  */
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


void manageInput(){
    string tmp;

    if(disableManagement==false && counter<0) {
        cout << "s (step)   c(continue)     n(n steps)      a(abort)" << endl;
        tmp.clear();
        getline(cin,tmp);
        if(tmp == "c"){
            disableManagement = true;
            return;
        }
        else if(tmp=="s") return;
        else if(tmp=="a") exit(EXIT_FAILURE);

        int i=0;
        while(tmp[i]){
            //if there is a letter in a string then string is not a number
            if(tmp[i]==' ') break;
            if(isalpha(tmp[i])){
                cerr << "Unrecognized operation..." << endl;
                return manageInput();
            }
            else i++;
        }
        counter=stoi(tmp);

    }
    counter--;
    return;
}

/** \function execute
  * \brief executes a single operation at a time
  * \param op the operation code
  * \param a the string corresponding to the first parameter
  * \param b the string corresponding to the second parameter
  * \param c the string corresponding to the third parameter
  * \param p4 boolean to indicate the presence of a fourth parameter
  */
void execute(string op, string a, string b, string c, bool p4){
    int p1,p2,p3;
    p1=get_int(a,6);    //retrieve first parameter
    if(p4==true) p2=get_int(b,12);      //the second parameter is more than 6 bits
    else if(!b.empty()) p2=get_int(b,6);        //the second parameter is 6 bits
    if(!c.empty()) p3=get_int(c,6);     //retrieve third parameter

    if(op=="ADD" || op=="SUB" || op=="MUL"){
        cout << op << "\t" << "R" << p1 << " R" << p2 << " R" << p3 << endl;
        manageInput();
        if(op=="ADD") regTable[p3]=regTable[p1]+regTable[p2];
        if(op=="SUB") regTable[p3]=regTable[p1]-regTable[p2];
        if(op=="MUL") regTable[p3]=regTable[p1]*regTable[p2];
        cout << "R" << p3 <<" = " << regTable[p3]<<endl;
    }
    else if(op=="ADD_I" || op=="SUB_I" || op=="MUL_I"){
        cout << op << "\t" << "R" << p1 << " #" << p2 << " R" << p3 << endl;
        manageInput();
        int value=0;
        if(op=="ADD_I") value=regTable[p1]+p2;
        if(op=="SUB_I") value=regTable[p1]-p2;
        if(op=="MUL_I") value=regTable[p1]*p2;
        regTable[p3]=value;
        cout << "R" << p3 <<" = " << regTable[p3]<<endl;
    }
    else if(op=="CLEAR" || op=="INCR" || op=="DECR"){
        cout << op << "\t" << "R" << p1 << endl;
        manageInput();
        if(op=="CLEAR") regTable[p1]=0;
        if(op=="INCR") regTable[p1]++;
        if(op=="DECR") regTable[p1]--;
        cout << "R" << p1 <<" = " << regTable[p1]<<endl;
    }

    else if(op=="IF=" || op=="IF>" || op=="IF<" || op=="IF>=" || op=="IF<="){
        cout << op << "\t" << "R" << p1<< " R" << p2 << " #"<< p3 << endl;
        manageInput();
        int tmp=regTable[p1]-regTable[p2];
        if(op=="IF=" && tmp==0) i+=p3-1;
        if(op=="IF>" && tmp>0) i+=p3-1;
        if(op=="IF<" && tmp<0) i+=p3-1;
        if(op=="IF>=" && tmp>=0) i+=p3-1;
        if(op=="IF<=" && tmp<=0) i+=p3-1;
        cout << "PC = " << i+1<<endl;

    }
    else if(op=="IF=0" || op=="IF>0" || op=="IF<0" || op=="IF>=0" || op=="IF<=0"){
        cout << op << "\t" << "R" << p1<<" #"<<p2<<endl;
        manageInput();
        if(op=="IF=0" && regTable[p1]==0) i+=p2-1;
        if(op=="IF>0" && regTable[p1]>0) i+=p2-1;
        if(op=="IF<0" && regTable[p1]<0) i+=p2-1;
        if(op=="IF>=0" && regTable[p1]>=0) i+=p2-1;
        if(op=="IF<=0" && regTable[p1]<=0) i+=p2-1;
        cout << "PC = " << i+1<<endl;

    }

    else if(op=="LOAD" || op=="STORE"){
        cout << op << "\t" << "R" << p1 << " R" << p2 << " R" << p3<<endl;
        manageInput();
        int addr=regTable[p1]+regTable[p2];
        if(op=="LOAD"){
            regTable[p3]=memTable[addr];
            cout << "R" << p3 << " = " << memTable[addr]<<endl;
        }
        if(op=="STORE"){
            memTable[addr]=regTable[p3];
            cout << "LOC " << addr << " = " << regTable[p3]<<endl;
        }
    }

    else if(op=="LOAD_I" || op=="STORE_I"){
        cout << op << "\t" << "R" << p1 << " #" << p2 << " R" << p3<<endl;
        manageInput();
        int addr=regTable[p1]+p2;
        if(op=="LOAD_I"){
            regTable[p3]=memTable[addr];
            cout << "R" << p3 << " = " << memTable[addr]<<endl;
        }
        if(op=="STORE_I"){
            memTable[addr]=regTable[p3];
            cout << "LOC " << p3 << " = " << regTable[p3]<<endl;
        }
        cin.ignore();
    }
    else if(op=="MOVE"){
        cout << op << "\t" << "R" << p1 << " R" << p2<<endl;
        manageInput();
        regTable[p2]=regTable[p1];
        cout << "R" << p2 << " = " << regTable[p2]<<endl;
    }
    else if(op=="MOVE_I"){
        cout << op << "\t#" << p1 << " R" << p2<<endl;
        manageInput();
        regTable[p2]=p1;
        cout << "R" << p2 << " = " << regTable[p2]<<endl;
    }
    else if(op=="GOTO"){
        cout << op << "\t#" << p1<<endl;
        manageInput();
        i+=p1-1;
        cout << "PC = "<<i+1<<endl;

    }


}

int main(int argc,  char** argv) {

    cout << "Loading optable..." << endl;
    loadOptable();
    cout << "Loading registers..." << endl;
    loadRegisters();
    cout << "Loading memory..." << endl;
    loadMemory();
    cout << "Loading code..." << endl;
    loadProgram(argv[1]);

    int terminate=0;
    i=loc;      //keeps track of address of current instruction
    string op;
    string p1;
    string p2;
    string p3;
    string p4;
    cout << "Starting execution..." << endl;

    while(true) {
        istringstream iss(memCode[i]);
        iss >> op;
        cout << op << " ";
        if(opTable[op]=="END"){
            cout <<":   END"<< endl;
            break;
        }
        /* No parameters */
        else if(opTable[op]=="NOP") {
            cout << ":\t" << opTable[op];
            cin.ignore();
            break;
        }
        /* 3 REG parameters */
        else if(opTable[op]=="ADD" || opTable[op]=="SUB" || opTable[op]=="MUL" || opTable[op]=="LOAD" \
                || opTable[op]=="STORE" || opTable[op]=="IF<" || opTable[op]=="IF>" || opTable[op]=="IF=" \
                || opTable[op]=="IF<=" || opTable[op]=="IF>="){
            //Retrieve parameters
            iss >> p1;
            iss >> p2;
            iss >> p3;
            cout << p1 << " "<< p2 << " " << p3 << ":\t";
            execute(opTable[op],p1,p2,p3,false);
        }
        /* 2 parameters */
        else if(opTable[op]=="IF<0" || opTable[op]=="IF>0" || opTable[op]=="IF=0" \
                || opTable[op]=="IF<=0" || opTable[op]=="IF>=0" || opTable[op]=="MOVE" \
                || opTable[op]=="MOVE_I"){
            //Retrieve parameters
            iss >> p1;
            iss >> p2;
            cout << p1 << " "<< p2 << ":\t";
            if(opTable[op]=="MOVE_I" && memCode[i].length()>29){ //Param p3 exists
                    iss >> p3;
                    cout << p3 << ":\t";
                    stringstream ss;
                    ss << p1 << p3;
                    p1 = ss.str();
                    execute(opTable[op],p1,p2,"",true);
                }
            else execute(opTable[op],p1,p2,"",false);
        }
        /* 3 parameters, one is immediate */
        else if(opTable[op]=="ADD_I" || opTable[op]=="SUB_I" || opTable[op]=="MUL_I" || opTable[op]=="LOAD_I" || opTable[op]=="STORE_I"){
            //Retrieve parameters
            iss >> p1;
            iss >> p2;
            iss >> p3;
            cout << p1 << " "<< p2 << " " << p3 << " ";
            if(memCode[i].length()>29){ //Param p4 exists
                iss >> p4;
                cout << p4 << ":\t";
                stringstream ss;
                ss << p2 << p4;
                p2 = ss.str();
                execute(opTable[op],p1,p2,p3,true);
            }
            else execute(opTable[op],p1,p2,p3,false);
        }
        /* 1 REG parameter */
        else if(opTable[op]=="CLEAR" || opTable[op]=="INCR" || opTable[op]=="DECR" || opTable[op]=="GOTO"){
            //Retrieve parameter
            iss >> p1;
            cout << p1 << ":\t";
            execute(opTable[op],p1,"","",false);
        }
        i++;
    }
    cout << endl;

    /* Print register output */
    for (map<int,int>::iterator it=regTable.begin(); it!=regTable.end(); ++it){
        cout << "R" << it->first << " => " << it->second << endl;
    }
    cout << endl;

    map<int,int>::iterator it=memTable.begin();
    int tmp;

    /* Print memory output */
    while(it!=memTable.end()){
        cout << "LOC " << it->first << " => " << it->second;
        tmp=it->first;
        it++;
        while(it->first==tmp+1){
            cout << " " << it->second;
            it++; tmp++;
        }

        cout << endl;
    }

    return 0;
}