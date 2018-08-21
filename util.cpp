#include <unordered_map>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <bitset>
#include "util.h"

using namespace std;

string last_string;                     //Contains the last identifier found
string tmp;                             //Used to manipulate register identifiers
vector<int> v;                          //Contains the values for memlocs
int lc;                                 //Location Counter
ofstream outfile ("assembled.txt");     //Contains the resulting binary code
unordered_map<string, string > opTable;
unordered_map<string, int> symTable;

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
    new_node->value=value;
    new_node->type=type;
    new_node->param1=p1;
    new_node->param2=p2;
    new_node->param3=p3;

    return new_node;
}

/** \function make_seq_node
  * \brief Creates a new node of type SEQ
  * \param seq the vector to be inserted into the node
  * \return new_node
  */
tree make_seq_node(vector<int> seq){
    tree new_node= new node;
    new_node->value=-1;
    new_node->seq=seq;
    new_node->type="SEQ";
    new_node->param1=NULL;
    new_node->param2=NULL;
    new_node->param3=NULL;

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
  * \return b.toString() the binary equivalent for v
  */
string get_param_binary(int v){
    bitset<16> b(v);
    return b.to_string();
}

/** \function visit_tree
  * \brief Recursively visits the syntax tree and generates binary values
  * \param tmp the current node to visit
  */
void visit_tree(node* tmp){

    if(tmp==NULL) return;
    if(tmp->type=="PROGRAM" || tmp->type=="INSTRUCTION"){
        visit_tree(tmp->param1);
        visit_tree(tmp->param2);
        visit_tree(tmp->param3);
    }
    else if(tmp->type=="ADD" || tmp->type=="SUB" || tmp->type=="MUL"){   //Expected 3 parameters
        if(tmp->type=="ADD"){
            if(tmp->param1->type=="REGISTER" && tmp->param2->type=="REGISTER" ){
                bitset<8> bin=get_opcode_binary("ADD_RRR");
                outfile << bin.to_string() << " ";
            }
            else if(tmp->param1->type=="VALUE" && tmp->param2->type=="REGISTER" ){
                bitset<8> bin=get_opcode_binary("ADD_IRR");
                outfile << bin.to_string() << " ";
            }
            else if(tmp->param1->type=="REGISTER" && tmp->param2->type=="VALUE" ){
                bitset<8> bin=get_opcode_binary("ADD_RIR");
                outfile << bin.to_string() << " ";
            }
            else if(tmp->param1->type=="VALUE" && tmp->param2->type=="VALUE" ){
                bitset<8> bin=get_opcode_binary("ADD_IIR");
                outfile << bin.to_string() << " ";
            }
        }

        if(tmp->type=="SUB"){
            if(tmp->param1->type=="REGISTER" && tmp->param2->type=="REGISTER" ){
                bitset<8> bin=get_opcode_binary("SUB_RRR");
                outfile << bin.to_string() << " ";
            }
            else if(tmp->param1->type=="VALUE" && tmp->param2->type=="REGISTER" ){
                bitset<8> bin=get_opcode_binary("SUB_IRR");
                outfile << bin.to_string() << " ";
            }
            else if(tmp->param1->type=="REGISTER" && tmp->param2->type=="VALUE" ){
                bitset<8> bin=get_opcode_binary("SUB_RIR");
                outfile << bin.to_string() << " ";
            }
            else if(tmp->param1->type=="VALUE" && tmp->param2->type=="VALUE" ){
                bitset<8> bin=get_opcode_binary("SUB_IIR");
                outfile << bin.to_string() << " ";
            }
        }

        if(tmp->type=="MUL"){
            if(tmp->param1->type=="REGISTER" && tmp->param2->type=="REGISTER" ){
                bitset<8> bin=get_opcode_binary("MUL_RRR");
                outfile << bin.to_string() << " ";
            }
            else if(tmp->param1->type=="VALUE" && tmp->param2->type=="REGISTER" ){
                bitset<8> bin=get_opcode_binary("MUL_IRR");
                outfile << bin.to_string() << " ";
            }
            else if(tmp->param1->type=="REGISTER" && tmp->param2->type=="VALUE" ){
                bitset<8> bin=get_opcode_binary("MUL_RIR");
                outfile << bin.to_string() << " ";
            }
            else if(tmp->param1->type=="VALUE" && tmp->param2->type=="VALUE" ){
                bitset<8> bin=get_opcode_binary("MUL_IIR");
                outfile << bin.to_string() << " ";
            }
        }

        string val=get_param_binary(tmp->param1->value);
        outfile << val << " ";
        val=get_param_binary(tmp->param2->value);
        outfile << val << " ";
        val=get_param_binary(tmp->param3->value);
        outfile << val<< endl;
        return;
    }
    else if(tmp->type=="EQ" || tmp->type=="LT" || tmp->type=="GT"){   //Expected 3 parameters, last one is a label
        if(tmp->type=="EQ"){
            if(tmp->param1->type=="REGISTER" && tmp->param2->type=="REGISTER" ){
                bitset<8> bin=get_opcode_binary("EQ_RR");
                outfile << bin.to_string() << " ";
            }
            else if(tmp->param1->type=="VALUE" && tmp->param2->type=="REGISTER" ){
                bitset<8> bin=get_opcode_binary("EQ_IR");
                outfile << bin.to_string() << " ";
            }
            else if(tmp->param1->type=="REGISTER" && tmp->param2->type=="VALUE" ){
                bitset<8> bin=get_opcode_binary("EQ_RI");
                outfile << bin.to_string() << " ";
            }
            else if(tmp->param1->type=="VALUE" && tmp->param2->type=="VALUE" ){
                bitset<8> bin=get_opcode_binary("EQ_II");
                outfile << bin.to_string() << " ";
            }
        }

        if(tmp->type=="LT"){
            if(tmp->param1->type=="REGISTER" && tmp->param2->type=="REGISTER" ){
                bitset<8> bin=get_opcode_binary("LT_RR");
                outfile << bin.to_string() << " ";
            }
            else if(tmp->param1->type=="VALUE" && tmp->param2->type=="REGISTER" ){
                bitset<8> bin=get_opcode_binary("LT_IR");
                outfile << bin.to_string() << " ";
            }
            else if(tmp->param1->type=="REGISTER" && tmp->param2->type=="VALUE" ){
                bitset<8> bin=get_opcode_binary("LT_RI");
                outfile << bin.to_string() << " ";
            }
            else if(tmp->param1->type=="VALUE" && tmp->param2->type=="VALUE" ){
                bitset<8> bin=get_opcode_binary("LT_II");
                outfile << bin.to_string() << " ";
            }
        }

        if(tmp->type=="GT"){
            if(tmp->param1->type=="REGISTER" && tmp->param2->type=="REGISTER" ){
                bitset<8> bin=get_opcode_binary("GT_RR");
                outfile << bin.to_string() << " ";
            }
            else if(tmp->param1->type=="VALUE" && tmp->param2->type=="REGISTER" ){
                bitset<8> bin=get_opcode_binary("GT_IR");
                outfile << bin.to_string() << " ";
            }
            else if(tmp->param1->type=="REGISTER" && tmp->param2->type=="VALUE" ){
                bitset<8> bin=get_opcode_binary("GT_RI");
                outfile << bin.to_string() << " ";
            }
            else if(tmp->param1->type=="VALUE" && tmp->param2->type=="VALUE" ){
                bitset<8> bin=get_opcode_binary("GT_II");
                outfile << bin.to_string() << " ";
            }
        }

        string val=get_param_binary(tmp->param1->value);
        outfile << val << " ";
        val=get_param_binary(tmp->param2->value);
        outfile << val << " ";
        int addr=find_symbol(tmp->param3->type);
        if(addr==-1) cout << "Error, symbol not in Symbol Table!" << endl;
        else {
            val = get_param_binary(addr);
            outfile << val << endl;
        }
        return;
    }
    else if(tmp->type=="LABEL"){
        int addr=find_symbol(tmp->param1->type);
        if(addr==-1) cout << "Error, symbol not in Symbol Table!" << endl;
        else visit_tree(tmp->param2);
    }
    else{
            bitset<8> bin=get_opcode_binary(tmp->type);
            outfile << bin.to_string() << " ";
        }


    if(tmp->type=="MEMLOC" || tmp->type=="REGVAL"){ //Expected two integer values

        string val=get_param_binary(tmp->param1->value);
        outfile << val << " ";
        val=get_param_binary(tmp->param2->value);
        outfile << val<< endl;
        return;

    }

    if(tmp->type=="LOC" || tmp->type=="START" || tmp->type=="CLEAR" || tmp->type=="INCR"){ //Expected one integer value
        string val=get_param_binary(tmp->param1->value);
        outfile << val << endl;
        return;
    }

    if(tmp->type=="LOAD" || tmp->type=="STORE"){ //Expected three values
        string val=get_param_binary(tmp->param1->value);
        outfile << val << " ";
        val=get_param_binary(tmp->param2->value);
        outfile << val << " ";
        val=get_param_binary(tmp->param3->value);
        outfile << val<< endl;
        return;
    }
    if(tmp->type=="MEMLOCS"){   //Multiple parameters
        string val=get_param_binary(tmp->param1->value);
        outfile << val << " ";

        for(int i=0; i<v.size(); i++){
            val=get_param_binary(v[i]);
            outfile << val << " ";
        }

        outfile << endl;
    }

    if(tmp->type=="GOTO"){
        int addr=find_symbol(tmp->param1->type);
        if(addr==-1) cout << "Error, symbol not in Symbol Table!" << endl;
        else {
            string val = get_param_binary(addr);
            outfile << val << endl;
        }
        return;
    }

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