#include <iostream>
#include <fstream>
#include <map>
#include <sstream>
#include <string>
#include <unordered_map>
#include <bitset>


using namespace std;

map<int,int> regTable;
map<int,int> memTable;
map<int,string> memCode;
unordered_map<string, string > opTable;
int start=-1;
int loc=-1;

/** \function get_opcode_binary
  * \brief generates the string containing the binary digits for the opcode
  * \param s the string
  * \return b.to_string() the string of binary digits
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
  * \brief reads the optable file and imports it into a map structure
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
  * \brief reads the registers file and imports it into a map structure
  */
void loadRegisters(){
    ifstream regFile("registers.o");
    if(!regFile){
	cerr << "Error, \"registers.o\" does not exist, relaunch parser..." << endl;
	exit(EXIT_FAILURE);
    }
    string value;
    string tmp;
    while (regFile >> value){
        tmp=value;
        regFile >> value;
        regTable[stoi(tmp)]=stoi(value);
    }
    regFile.close();
}

/** \function loadMemory
  * \brief reads the memory file and imports it into a map structure
  */
void loadMemory(){
    ifstream memFile("memory.o");
    if(!memFile){
	cerr << "Error, \"memory.o\" does not exist, relaunch parser..." << endl;
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
            loc=stoi(value);
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
  * \brief reads the code file and imports it into a map structure
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
    int memptr=loc;
    while (getline(codeFile, instruction)) {
        memCode[memptr]=instruction;
        memptr++;
    }
    codeFile.close();
}
