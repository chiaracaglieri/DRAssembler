/**
 * @file interpreter.cpp
 * @author Chiara Caglieri
 * @brief The interpreter's main file
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include "int_util.h"

using namespace std;

int i;                          /**<Current instruction*/
bool disableManagement=false;   /**<Flag to enable display of prompt after instruction*/
int counter=-1;                 /**<Keeps track of the number of instructions ledt to execute after command "n steps" entered*/

/** \function get_int
  * @brief generates the int corresponding to a string of binary digits
  * @param s the string
  * @param nbits the number of bits in the string
  * @return (int) b.to_ulong() the integer translation of the digits
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
/** \function remove_spaces
  * @brief removes spaces in a string
  * @param input
  * @return input the string without spaces
  */
string remove_spaces(string input) {
    input.erase(remove(input.begin(),input.end(),' '),input.end());
    return input;
}

/** \function manageInput
 *  @brief displays the interactive options on the command line
 */
void manageInput(){
    string tmp;

    if(disableManagement==false && counter<0) {
        cout << "enter (step)   c(continue)     n(n steps)      a(abort)" << endl;
        tmp.clear();
        getline(cin,tmp);
        if(tmp == "c"){
            disableManagement = true;
            return;
        }
        else if(tmp=="a") exit(EXIT_FAILURE);
        else if(tmp=="") return;
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

/** \function hasImmediate
  * @brief determines whether the operation has an immediate parameter
  * @param op the operation to check
  * @return true if the op. has an immediate value
  *         false otherwise
 */
bool hasImmediate(string op){
    if(op=="ADD_I" || op=="SUB_I" || op=="MUL_I" \
    || op=="LOAD_I" || op=="STORE_I" || op=="DIV_I") return true;
    return false;
}

/** \function isConditional
  * @brief determines whether the operation is of conditional type
  * @param op the operation to check
  * @return true if the op. is of conditional type
  *         false otherwise
 */
bool isConditional(string op){
    if(op=="IF="      || \
       op=="IF>"      || \
       op=="IF<"      || \
       op=="IF>="     || \
       op=="IF<="     || \
       op=="IF!=")
        return true;
    return false;
}

/** \function isConditional0
  * @brief determines whether the operation is of conditional_0 type
  * @param op the operation to check
  * @return true if the op. is of conditional_0 type
  *         false otherwise
 */
bool isConditional0(string op){
    if(op=="IF=0"      || \
       op=="IF>0"      || \
       op=="IF<0"      || \
       op=="IF>=0"     || \
       op=="IF<=0"     || \
       op=="IF!=0")
        return true;
    return false;
}

/** \function execute
  * @brief executes a single operation at a time
  * @param op the operation code
  * @param a the string corresponding to the first parameter
  * @param b the string corresponding to the second parameter
  * @param c the string corresponding to the third parameter
  * @param p4 boolean to indicate the presence of a fourth parameter
  */
void execute(string op, string a, string b, string c, string d){
    int p1,p2,p3,p4;
    if(hasImmediate(op)){
        p1=get_int(a,6);    //retrieve first parameter
        string tmp=b.append(d);
        p2=get_int(tmp,12);
        p3=get_int(c,6);
    }
    else if(op=="MOVE_I"){
        string tmp=a.append(d);
        p1=get_int(tmp,12);
        p2=get_int(b,6);
    }
    else if(isConditional(op)){
        p1=get_int(a,6);
        p2=get_int(b,6);
        string tmp=c.append(d);
        p3=get_int(tmp,12);
    }
    else if(isConditional0(op)){
        p1=get_int(a,6);
        string tmp=b.append(d);
        p2=get_int(tmp,12);
    }
    else if(op=="GOTO_I"){
        string tmp=a.append(d);
        p1=get_int(tmp,12);
    }
    else if(op=="GOTO"){
        p1=get_int(a,6);
    }
    else if(op=="CALL"){
        p1=get_int(a,6);
        p2=get_int(b,6);
    }
    else{
        p1=get_int(a,6);
        p2=get_int(b,6);
        p3=get_int(c,6);
    }

    if(op=="ADD" || op=="SUB" || op=="MUL" || op=="DIV"){
        cout << op << "\t" << "R" << p1 << " R" << p2 << " R" << p3 << endl;
        manageInput();
        if(op=="ADD") regTable[p3]=regTable[p1]+regTable[p2];
        if(op=="SUB") regTable[p3]=regTable[p1]-regTable[p2];
        if(op=="MUL") regTable[p3]=regTable[p1]*regTable[p2];
        if(op=="DIV") regTable[p3]=regTable[p1]/regTable[p2];
        cout << "R" << p3 <<" = " << regTable[p3]<<endl;
    }
    else if(op=="ADD_I" || op=="SUB_I" || op=="MUL_I" || op=="DIV_I"){
        cout << op << "\t" << "R" << p1 << " #" << p2 << " R" << p3 << endl;
        manageInput();
        int value=0;
        if(op=="ADD_I") value=regTable[p1]+p2;
        if(op=="SUB_I") value=regTable[p1]-p2;
        if(op=="MUL_I") value=regTable[p1]*p2;
        if(op=="DIV_I") value=regTable[p1]/p2;
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

    else if(op=="IF=" || op=="IF>" || op=="IF<" || op=="IF>=" || op=="IF<=" || op=="IF!="){
        cout << op << "\t" << "R" << p1<< " R" << p2 << " #"<< p3 << endl;
        manageInput();
        int tmp=regTable[p1]-regTable[p2];
        if(op=="IF=" && tmp==0) i+=p3-1;
        if(op=="IF>" && tmp>0) i+=p3-1;
        if(op=="IF<" && tmp<0) i+=p3-1;
        if(op=="IF>=" && tmp>=0) i+=p3-1;
        if(op=="IF<=" && tmp<=0) i+=p3-1;
        if(op=="IF!=" && tmp!=0) i+=p3-1;
        cout << "PC = " << i+1<<endl;

    }
    else if(op=="IF=0" || op=="IF>0" || op=="IF<0" || op=="IF>=0" || op=="IF<=0" || op=="IF!=0"){
        cout << op << "\t" << "R" << p1<<" #"<<p2<<endl;
        manageInput();
        if(op=="IF=0" && regTable[p1]==0) i+=p2-1;
        if(op=="IF>0" && regTable[p1]>0) i+=p2-1;
        if(op=="IF<0" && regTable[p1]<0) i+=p2-1;
        if(op=="IF>=0" && regTable[p1]>=0) i+=p2-1;
        if(op=="IF<=0" && regTable[p1]<=0) i+=p2-1;
        if(op=="IF!=0" && regTable[p1]!=0) i+=p2-1;
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
            cout << "pc " << addr << " = " << regTable[p3]<<endl;
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
            cout << "pc " << p3 << " = " << regTable[p3]<<endl;
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
    else if(op=="GOTO_I"){
        cout << op << "\t#" << p1<<endl;
        manageInput();
        i+=p1-1;
        cout << "PC = "<<i+1<<endl;

    }
    else if(op=="GOTO"){
        cout << op << "\tR" << p1<<endl;
        manageInput();
        i=regTable[p1];
        cout << "PC = "<<i+1<<endl;

    }
    else if(op=="CALL"){
        cout << op << "\tR" << p1<< " R" << p2<<endl;
        manageInput();
        regTable[p2]=i;
        i=regTable[p1]-1;
        cout << "R" << p2 << " = " << regTable[p2]<<endl;
        cout << "PC = "<<i<<endl;

    }

}

int main(int argc,  char** argv) {

    cout << "Loading optable..." << endl;
    loadOptable();
    cout << "Loading registers..." << endl;
    loadRegisters(argv[1]);
    cout << "Loading memory..." << endl;
    loadMemory(argv[1]);
    cout << "Loading code..." << endl;
    loadProgram(argv[1]);

    int terminate=0;
    i=pc;      //keeps track of address of current instruction
    string op;
    string p1;
    string p2;
    string p3;
    string p4;
    cout << "Starting execution..." << endl;

    while(true) {
        cout << memCode[i] << " :\t";
        string current=remove_spaces(memCode[i]);
        op=current.substr(0,8);
        //Retrieve parameters
        p1=current.substr(8,6);
        p2=current.substr(14,6);
        p3=current.substr(20,6);
        p4=current.substr(26,6);

        if(opTable[op]=="END") {cout<<"END"<<endl; break;}
        if(opTable[op]!="NOP") execute(opTable[op],p1,p2,p3,p4);
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
        cout << "pc " << it->first << " => " << it->second;
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