//
// Created by chiara on 24/09/18.
//
#include <iostream>
#include <fstream>
#include <map>
#include <sstream>
#include <string>
#include <unordered_map>
#include <bitset>
//#include "state.h"

using namespace std;

ifstream regFile("registers.txt");
ifstream memFile("memory.txt");
ifstream codeFile("assembled.txt");


map<int,int> regTable;
map<int,string> memTable;
unordered_map<string, string > opTable;
int start=-1;
int loc=-1;

string get_opcode_binary(string s){
    stringstream ss;
    ss << hex << s;
    unsigned n;
    ss >> n;
    bitset<8> b(n);
    return b.to_string();
}

void loadOptable(){
    ifstream opcodes ("optable.txt");

    string word;
    while(opcodes >> word){
        string op=word;
        opcodes >> word;
        string b=get_opcode_binary(word);
        opTable[b]=op;
    }

    cout << "im done" << endl;
}


void loadRegisters(){

    string value;
    string tmp;
    while (regFile >> value){
        tmp=value;
        regFile >> value;
        regTable[stoi(tmp)]=stoi(value);
    }
    regFile.close();
}
void loadMemory(){
    string value;
    string tmp;
    while (memFile >> value){
        if(value=="s"){     //This is the start position
            memFile >> value;
            start=stoi(value);
        }
        else if(value=="l"){
            memFile >> value;
            loc=stoi(value);
        }
        else {
            tmp = value;
            memFile >> value;
            memTable[stoi(tmp)] = value;
        }
    }
    memFile.close();
}

void loadProgram(){
    string instruction;
    int memptr=loc;
    while (getline(codeFile, instruction)) {
        memTable[memptr]=instruction;
        memptr++;
    }
    codeFile.close();
}