/**
 * @file  int_util.cpp
 * @author Chiara Caglieri
 * @brief File containing the implementation of utility functions for the interpreter
 */

#include <iostream>
#include <fstream>
#include <map>
#include <sstream>
#include <string>
#include <unordered_map>
#include <bitset>


using namespace std;

map<int,int> regTable;
map<int,string> symRegTable;
map<int,int> memTable;
map<int,string> memCode;
unordered_map<string, string > opTable;
int start=-1;
int pc=-1;

/** \function get_opcode_binary
  * @brief generates the string containing the binary digits for the opcode
  * @param s the string
  * @return b.to_string() the string of binary digits
  */
string get_opcode_binary(string s){
    stringstream ss;
    ss << hex << s;
    unsigned n;
    ss >> n;
    bitset<8> b(n);
    return b.to_string();
}
/** \function loadOptable
  * @brief reads the optable file and imports it into a map structure
  */
void loadOptable(){
    ifstream opcodes("optable.txt");
    if(!opcodes){
	cerr << "Error, \"optable.txt\" does not exist..." << endl;
	exit(EXIT_FAILURE);
    }
    string word;
    while(opcodes >> word){
        string op=word;
        opcodes >> word;
        string b=get_opcode_binary(word);
        opTable[b]=op;
    }

}

/** \function loadRegisters
  * @brief reads the registers file and imports it into a map structure
  */
void loadRegisters(string fname){
    string out = fname.substr(0, fname.size()-2);
    out.append("-regs.o");

    ifstream regFile(out);
    if(!regFile){
	cerr << "Error,"<<out<<" does not exist, relaunch parser..." << endl;
	exit(EXIT_FAILURE);
    }
    string value;
    string tmp;
    while (regFile >> value){
        if(value=="#"){
            /*Symbolic register association found*/
            regFile >> value;
            tmp=value;
            regFile >> value;
            symRegTable[stoi(value.substr(1,value.size()-1))]=tmp;
        }
        else {
            tmp = value;
            regFile >> value;
            regTable[stoi(tmp)] = stoi(value);
        }
    }
    regFile.close();
}

/** \function loadMemory
  * @brief reads the memory file and imports it into a map structure
  */
void loadMemory(string fname){
    string out = fname.substr(0, fname.size()-2);
    out.append("-mem.o");

    ifstream memFile(out);
    if(!memFile){
	cerr << "Error,"<<out<<" does not exist, relaunch parser..." << endl;
	exit(EXIT_FAILURE);
    }
    string value;
    string tmp;
    while (memFile >> value){
        if(value=="s"){     //This is the start position
            memFile >> value;
            start=stoi(value);
        }
        else if(value=="l"){
            memFile >> value;
            pc=stoi(value);
        }
        else {
            tmp = value;
            memFile >> value;
            memTable[stoi(tmp)] = stoi(value);
        }
    }
    memFile.close();
}

/** \function loadProgram
  * @brief reads the code file and imports it into a map structure
  */
void loadProgram(string filename){
    /*Check file type*/
    if(filename.substr(filename.find_last_of(".") + 1) != "o") {
        cerr << "Error,  \""<< filename << "\" is not of type .o..." << endl;
        exit(EXIT_FAILURE);
    }
    ifstream codeFile(filename);
    if(!codeFile){
	cerr << "Error,  \""<< filename << "\" does not exist,relaunch parser..." << endl;
	exit(EXIT_FAILURE);
    }
    string instruction;
    int memptr=pc;
    while (getline(codeFile, instruction)) {
        memCode[memptr]=instruction;
        memptr++;
    }
    codeFile.close();
}

string checkSym(int reg){
    if(symRegTable.find(reg) == symRegTable.end() ) {
        stringstream s;
        s << "R"<< reg;
        return s.str();
    }
    else return symRegTable[reg];
}