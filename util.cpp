#include <unordered_map>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <bitset>
#include "util.h"

using namespace std;

string last_string;                     //Contains the last identifier found
string tmp;                             //Used to manipulate register identifiers
deque<int> v;                           //Contains the values for memlocs
int lc;                                 //Location Counter
ofstream outfile;
vector<string> words;

ofstream reg_out ("registers.o");
ofstream mem_out ("memory.o");

unordered_map<string, string > opTable;
unordered_map<string, int> symTable;

void initRegister(int r, int v){
    reg_out << r << "\t";
    reg_out << v <<endl;
}
void initMemloc(int l, int v){
    mem_out << l << "\t";
    mem_out << v <<endl;
}
void initMemlocs(int l, deque<int> v){

    for(int i=0; i<v.size(); i++){
        mem_out << l << "\t";
        mem_out << v[i] << endl;
        l++;
    }
}

/*  Symbol Table functions  */
/** \function find_symbol
  * \brief Searches for symbol in Symbol Table.
  * \param sym the symbol to be found
  * \return symTable[sym] value of lc associated with sym
  *         -1 if unsuccessful
  */
int find_symbol(string sym){
    if(symTable.find(sym)!=symTable.end())
        return symTable[sym];
    else return -1;
}

/** \function insert_symbol
  * \brief Inserts symbol in Symbol Table.
  * \param sym the symbol to be inserted
  * \param lc the location counter associated with the symbol
  *        (-1 if LC is not relevant)
  */

void insert_symbol(string sym, int lc){

    if(find_symbol(sym)!=-1) cout << "Symbol " << sym << " is already in Symbol Table" << endl;
    else {symTable[sym]=lc; cout << "Symbol " << sym << " inserted at position " << lc << endl;}
}

/*Syntax Tree Functions    */

tree SynTree=NULL;

/** \function make_node
  * \brief Creates a new node
  * \param type the type of node
  * \param value the associated value
  * \param p1 pointer to first child node
  * \param p2 pointer to second child node
  * \param p3 pointer to third child node
  * \return new_node
  */
tree make_node(string type, int value, node* p1, node* p2, node* p3){
    tree new_node= new node;
    //Check if value is representable
    if(type=="REGISTER" && value>63){   //Register numbers 0...63 not in complement
        cout << value << " is not representable on 6 bits, aborting..." << "\n";
        exit(EXIT_FAILURE);
    }
    if(type!="REGISTER" && value>2047){ //1 bit for positive/negative
        cout << value << " is not representable on 12 bits, aborting..." << "\n";
        exit(EXIT_FAILURE);
    }
    new_node->value=value;
    new_node->lc=lc;
    new_node->type=type;
    new_node->param1=p1;
    new_node->param2=p2;
    new_node->param3=p3;
    return new_node;
}

/** \function append_tree
  * \brief Istantiates the root of the tree
  * \param node the node to be set as the root
  */
void append_tree(node* node) {
    SynTree = node;
}

/** \function get_opcode_binary
  * \brief Generates binary value for hex opcode
  * \param s the type of instruction
  * \return b the binary opcode
  */
bitset<8> get_opcode_binary(string s){
    stringstream ss;
    ss << hex << opTable[s];
    unsigned n;
    ss >> n;
    bitset<8> b(n);
    return b;
}

/** \function get_param_binary
  * \brief Generates binary value for int value
  * \param v the int value
  * \return b the binary equivalent for v
  */
string get_param_binary(int v){
    bitset<6> b(v);
    return b.to_string();

}

string get_constant_binary(int v){

    bitset<12> b(v);
    return b.to_string();
}

/** \function visit_tree
  * \brief Recursively visits the syntax tree and generates binary values
  * \param tmp the current node to visit
  */
void visit_tree(node* tmp){
    string instruction;

    if(tmp==NULL) return;

    if(tmp->type=="PROGRAM" || tmp->type=="INSTRUCTION"){
        visit_tree(tmp->param1);
        visit_tree(tmp->param2);
        visit_tree(tmp->param3);
    }
    else if(tmp->type=="ADD" || tmp->type=="SUB" || tmp->type=="MUL"){   //Expected 3 parameters
        if(tmp->type=="ADD"){
            if(tmp->param2->type=="VALUE"){
                bitset<8> bin=get_opcode_binary("ADD_I");
                instruction.append(bin.to_string());
            }
            else{
                bitset<8> bin = get_opcode_binary("ADD");
                instruction.append(bin.to_string());
            }
        }

        if(tmp->type=="SUB"){
            if(tmp->param2->type=="VALUE"){
                bitset<8> bin=get_opcode_binary("SUB_I");
                instruction.append(bin.to_string());
            }
            else{
                bitset<8> bin = get_opcode_binary("SUB");
                instruction.append(bin.to_string());
            }
        }

        if(tmp->type=="MUL"){
            if(tmp->param2->type=="VALUE"){
                bitset<8> bin=get_opcode_binary("MUL_I");
                instruction.append(bin.to_string());
            }
            else{
                bitset<8> bin = get_opcode_binary("MUL");
                instruction.append(bin.to_string());
            }
        }

        string val=get_param_binary(tmp->param1->value);
        instruction.append(val);
        if(tmp->param2->type=="VALUE"){
            if(tmp->param2->value<=2047 && tmp->param2->value>=-2048){   //representable on 12 bits two-complement
                string val_long=get_constant_binary(tmp->param2->value);
                instruction.append(val_long.substr(0,6));
                val=get_param_binary(tmp->param3->value);
                instruction.append(val);
                instruction.append(val_long.substr(6,12));
            }
            else{
                cerr << "Error: "<< tmp->param2->value <<" is not representable on 12 bits 2-complement"<<endl;
                exit(EXIT_FAILURE);
            }
        }
        else{
            val=get_param_binary(tmp->param2->value);
            instruction.append(val);
            val=get_param_binary(tmp->param3->value);
            instruction.append(val);
            val=get_param_binary(0);
            instruction.append(val);
        }

        words.push_back(instruction);
        return;
    }
    else if(tmp->type=="IF=" || tmp->type=="IF<" || tmp->type=="IF>" \
            || tmp->type=="IF<=" || tmp->type=="IF>="){   //Expected 3 parameters, last one is a label

        bitset<8> bin=get_opcode_binary(tmp->type);
        instruction.append(bin.to_string());

        string val=get_param_binary(tmp->param1->value);
        instruction.append(val);
        val=get_param_binary(tmp->param2->value);
        instruction.append(val);
        int addr=find_symbol(tmp->param3->type);

        if(addr==-1) cout << "Error, symbol not in Symbol Table!" << endl;
        else {
            int relative=addr-tmp->lc;

            if(relative<=2047 && relative>=-2048){   //representable on 12 bits two-complement
                string val_long=get_constant_binary(relative);
                instruction.append(val_long.substr(0,6));
                instruction.append(val_long.substr(6,12));
            }
            else{
                cerr << "Error: "<< relative <<" is not representable on 12 bits 2-complement"<<endl;
                exit(EXIT_FAILURE);
            }
        }
        words.push_back(instruction);
        return;
    }
    else if(tmp->type=="MOVE"){  //expected 2 parameters
        bitset<8> bin;
        if(tmp->param1->type=="VALUE") bin= get_opcode_binary("MOVE_I");
        else  bin = get_opcode_binary("MOVE");
        instruction.append(bin.to_string());

        if(tmp->param1->type=="VALUE"){
            if(tmp->param1->value<=2047 && tmp->param1->value>=-2048){   //representable on 12 bits two-complement
                string val_long=get_constant_binary(tmp->param1->value);
                instruction.append(val_long.substr(0,6));
                string val=get_param_binary(tmp->param2->value);
                instruction.append(val);
                val=get_param_binary(0);
                instruction.append(val);
                instruction.append(val_long.substr(6,12));
            }
            else{
                cerr << "Error: "<< tmp->param1->value <<" is not representable on 12 bits 2-complement"<<endl;
                exit(EXIT_FAILURE);
            }
        }
        else{
            string val=get_param_binary(tmp->param1->value);
            instruction.append(val);
            val=get_param_binary(tmp->param2->value);
            instruction.append(val);
            val=get_param_binary(0);
            instruction.append(val);
            instruction.append(val);
        }


        words.push_back(instruction);
        return;
    }
    else if(tmp->type=="IF>0" || tmp->type=="IF=0" || tmp->type=="IF<0" || tmp->type=="IF<=0" \
            || tmp->type=="IF>=0"){ //2 parameters. 2nd is a label
        bitset<8> bin = get_opcode_binary(tmp->type);
        instruction.append(bin.to_string());

        string val=get_param_binary(tmp->param1->value);
        instruction.append(val);
        int addr=find_symbol(tmp->param2->type);
        if(addr==-1) cout << "Error, symbol not in Symbol Table!" << endl;
        else {
            int relative=addr-tmp->lc;
            if(relative<=2047 && relative>=-2048){   //representable on 12 bits two-complement
                string val_long=get_constant_binary(relative);
                instruction.append(val_long.substr(0,6));
                instruction.append(val_long.substr(6,12));
            }
            else{
                cerr << "Error: "<< relative <<" is not representable on 12 bits 2-complement"<<endl;
                exit(EXIT_FAILURE);
            }
        }
        val=get_param_binary(0);
        instruction.append(val);
        words.push_back(instruction);
        return;
    }
    else if(tmp->type=="LABEL"){
        int addr=find_symbol(tmp->param1->type);
        if(addr==-1) cout << "Error, symbol not in Symbol Table!" << endl;
        else visit_tree(tmp->param2);
    }

    else if(tmp->type=="LOAD"){
        if(tmp->param2->type=="VALUE"){
            bitset<8> bin=get_opcode_binary("LOAD_I");
            instruction.append(bin.to_string());
        }
        else {
            bitset<8> bin = get_opcode_binary("LOAD");
            instruction.append(bin.to_string());
        }

        string val=get_param_binary(tmp->param1->value);
        instruction.append(val);
        if(tmp->param2->type=="VALUE"){
            if(tmp->param2->value<=2047 && tmp->param2->value>=-2048){   //representable on 12 bits two-complement
                string val_long=get_constant_binary(tmp->param2->value);
                instruction.append(val_long.substr(0,6));
                val=get_param_binary(tmp->param3->value);
                instruction.append(val);
                instruction.append(val_long.substr(6,12));
            }
            else{
                cerr << "Error: "<< tmp->param2->value <<" is not representable on 12 bits 2-complement"<<endl;
                exit(EXIT_FAILURE);
            }
        }
        else{
            val=get_param_binary(tmp->param2->value);
            instruction.append(val);
            val=get_param_binary(tmp->param3->value);
            instruction.append(val);
            val=get_param_binary(0);
            instruction.append(val);
        }

        words.push_back(instruction);
        return;
    }

    else if(tmp->type=="STORE"){ //Expected three values
        if(tmp->param2->type=="VALUE"){
            bitset<8> bin=get_opcode_binary("STORE_I");
            instruction.append(bin.to_string());
        }
        else{
            bitset<8> bin=get_opcode_binary("STORE");
            instruction.append(bin.to_string());
        }

        string val=get_param_binary(tmp->param1->value);
        instruction.append(val);
        if(tmp->param2->type=="VALUE"){
            if(tmp->param2->value<=2047 && tmp->param2->value>=-2048){   //representable on 12 bits two-complement
                string val_long=get_constant_binary(tmp->param2->value);
                instruction.append(val_long.substr(0,6));
                val=get_param_binary(tmp->param3->value);
                instruction.append(val);
                instruction.append(val_long.substr(6,12));
            }
            else{
                cerr << "Error: "<< tmp->param2->value <<" is not representable on 12 bits 2-complement"<<endl;
                exit(EXIT_FAILURE);
            }
        }
        else{
            val=get_param_binary(tmp->param2->value);
            instruction.append(val);
            val=get_param_binary(tmp->param3->value);
            instruction.append(val);
            val=get_param_binary(0);
            instruction.append(val);
        }

        words.push_back(instruction);
        return;
    }

    else{
            bitset<8> bin=get_opcode_binary(tmp->type);
            instruction.append(bin.to_string());
    }
    if(tmp->type=="END"){
        string val=get_param_binary(0);
        instruction.append(val);
        instruction.append(val);
        instruction.append(val);
        instruction.append(val);
        words.push_back(instruction);
        return;
    }
    if(tmp->type=="CLEAR" || tmp->type=="INCR"  || tmp->type=="DECR"){ //Expected one integer value
        string val=get_param_binary(tmp->param1->value);
        instruction.append(val);
        val=get_param_binary(0);
        instruction.append(val);
        instruction.append(val);
        instruction.append(val);
        words.push_back(instruction);
        return;
    }
    if(tmp->type=="NOP"){
        bitset<8> bin=get_opcode_binary(tmp->type);
        string val=get_param_binary(0);
        instruction.append(val);
        instruction.append(val);
        instruction.append(val);
        instruction.append(val);
        words.push_back(instruction);
        return;
    }
    if(tmp->type=="GOTO"){
        int addr=find_symbol(tmp->param1->type);
        if(addr==-1) cout << "Error, symbol not in Symbol Table!" << endl;
        else {
            int relative=addr-tmp->lc;
            if(relative<=2047 && relative>=-2048){   //representable on 12 bits two-complement
                string val_long=get_constant_binary(relative);
                instruction.append(val_long.substr(0,6));
                string val=get_param_binary(0);
                instruction.append(val);
                instruction.append(val);
                instruction.append(val_long.substr(6,12));
            }
            else{
                cerr << "Error: "<< relative <<" is not representable on 12 bits 2-complement"<<endl;
                exit(EXIT_FAILURE);
            }
        }
        words.push_back(instruction);
        return;
    }

}

void substitute(string f, string p1, string p2){
    ifstream file(f);
    string contents;
    for (char ch; file.get(ch); contents.push_back(ch)) {}
    auto pos = contents.find(p1);
    pos = contents.find(p1,pos+1);  //avoid #define line
    while (pos != std::string::npos) {
        contents.replace(pos, p1.length(), p2);
        pos = contents.find(p1, pos);
    }
    ofstream ofile(f);
    ofile << contents;
}

/** \function load_optable
  * \brief Reads opcodes from text file and insertes them into a
  *        map structure
  */
void load_optable(){
    ifstream opcodes ("optable.txt");

    string word;
    while(opcodes >> word){
        string op=word;
        opcodes >> word;
        opTable[op]=word;
    }
}
