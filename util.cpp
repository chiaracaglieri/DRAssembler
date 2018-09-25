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
deque<int> v;                           //Contains the values for memlocs
int lc;                                 //Location Counter
ofstream outfile ("assembled.txt");     //Contains the resulting binary code

ofstream reg_out ("registers.txt");
ofstream mem_out ("memory.txt");

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
                outfile << bin.to_string() << " ";
            }
            else{
                bitset<8> bin = get_opcode_binary("ADD");
                outfile << bin.to_string() << " ";
            }
        }

        if(tmp->type=="SUB"){
            if(tmp->param2->type=="VALUE"){
                bitset<8> bin=get_opcode_binary("SUB_I");
                outfile << bin.to_string() << " ";
            }
            else{
                bitset<8> bin = get_opcode_binary("SUB");
                outfile << bin.to_string() << " ";
            }
        }

        if(tmp->type=="MUL"){
            if(tmp->param2->type=="VALUE"){
                bitset<8> bin=get_opcode_binary("MUL_I");
                outfile << bin.to_string() << " ";
            }
            else{
                bitset<8> bin = get_opcode_binary("MUL");
                outfile << bin.to_string() << " ";
            }
        }

        string val=get_param_binary(tmp->param1->value);
        outfile << val << " ";
        if(tmp->param2->type=="VALUE"){
            if(tmp->param2->value<=63){ //representable on 6 bits two-complement
                val=get_param_binary(tmp->param2->value);
                outfile << val << " ";
                val=get_param_binary(tmp->param3->value);
                outfile << val<< endl;
            }
            else if(tmp->param2->value<=2047){   //representable on 12 bits two-complement
                string val_long=get_constant_binary(tmp->param2->value);
                outfile << val_long.substr(0, 6) << " ";
                val=get_param_binary(tmp->param3->value);
                outfile << val<< " ";
                outfile << val_long.substr(6, 12) << endl;
            }
        }
        else{
            val=get_param_binary(tmp->param2->value);
            outfile << val << " ";
            val=get_param_binary(tmp->param3->value);
            outfile << val<< endl;
        }

        return;
    }
    else if(tmp->type=="EQ" || tmp->type=="LT" || tmp->type=="GT"){   //Expected 3 parameters, last one is a label
        if(tmp->type=="EQ"){
            if(tmp->param2->type=="VALUE"){
                bitset<8> bin=get_opcode_binary("EQ_0");
                outfile << bin.to_string() << " ";
            }
            else{
                bitset<8> bin=get_opcode_binary("EQ");
                outfile << bin.to_string() << " ";
            }
        }

        if(tmp->type=="LT"){
            if(tmp->param2->type=="VALUE"){
                bitset<8> bin=get_opcode_binary("LT_0");
                outfile << bin.to_string() << " ";
            }
            else{
                bitset<8> bin=get_opcode_binary("LT");
                outfile << bin.to_string() << " ";
            }
        }

        if(tmp->type=="GT"){
            if(tmp->param2->type=="VALUE"){
                bitset<8> bin=get_opcode_binary("GT_0");
                outfile << bin.to_string() << " ";
            }
            else{
                bitset<8> bin=get_opcode_binary("GT");
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
            int relative=addr-tmp->lc;
            cout << relative << endl;
            if(relative<=63){ //representable on 6 bits two-complement
                    val=get_param_binary(relative);
                    cout << val << endl;
                    outfile << val << endl;
                }
            else if(relative<=2047){   //representable on 12 bits two-complement
                    string val_long=get_constant_binary(relative);
                    outfile << val_long.substr(0, 6) << " ";
                    outfile << val_long.substr(6, 12) << endl;
            }
        }
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
            outfile << bin.to_string() << " ";
        }
        else {
            bitset<8> bin = get_opcode_binary("LOAD");
            outfile << bin.to_string() << " ";
        }

        string val=get_param_binary(tmp->param1->value);
        outfile << val << " ";
        if(tmp->param2->type=="VALUE"){
            if(tmp->param2->value<=63){ //representable on 6 bits two-complement
                val=get_param_binary(tmp->param2->value);
                outfile << val << " ";
            }
            else if(tmp->param2->value<=2047){   //representable on 12 bits two-complement
                string val_long=get_constant_binary(tmp->param2->value);
                outfile << val_long.substr(0, 6) << " ";
                val=get_param_binary(tmp->param3->value);
                outfile << val<< " ";
                outfile << val_long.substr(6, 12) << endl;
            }
        }
        else{
            val=get_param_binary(tmp->param2->value);
            outfile << val << " ";
            val=get_param_binary(tmp->param3->value);
            outfile << val<< endl;
        }

        return;
    }

    else if(tmp->type=="STORE"){ //Expected three values
        if(tmp->param2->type=="VALUE"){
            bitset<8> bin=get_opcode_binary("STORE_I");
            outfile << bin.to_string() << " ";
        }
        else{
            bitset<8> bin=get_opcode_binary("STORE");
            outfile << bin.to_string() << " ";
        }

        string val=get_param_binary(tmp->param1->value);
        outfile << val << " ";
        if(tmp->param2->type=="VALUE"){
            if(tmp->param2->value<=63){ //representable on 6 bits two-complement
                val=get_param_binary(tmp->param2->value);
                outfile << val << " ";
            }
            else if(tmp->param2->value<=2047){   //representable on 12 bits two-complement
                string val_long=get_constant_binary(tmp->param2->value);
                outfile << val_long.substr(0, 6) << " ";
                val=get_param_binary(tmp->param3->value);
                outfile << val<< " ";
                outfile << val_long.substr(6, 12) << endl;
            }
        }
        else{
            val=get_param_binary(tmp->param2->value);
            outfile << val << " ";
            val=get_param_binary(tmp->param3->value);
            outfile << val<< endl;
        }
        val=get_param_binary(tmp->param3->value);
        outfile << val<< endl;
        return;
    }
    else{
            bitset<8> bin=get_opcode_binary(tmp->type);
            outfile << bin.to_string() << " ";
    }

    if(tmp->type=="CLEAR" || tmp->type=="INCR"  || tmp->type=="DECR"){ //Expected one integer value
        string val=get_param_binary(tmp->param1->value);
        outfile << val << endl;
        return;
    }

    if(tmp->type=="GOTO"){
        int addr=find_symbol(tmp->param1->type);
        if(addr==-1) cout << "Error, symbol not in Symbol Table!" << endl;
        else {
            if(addr<=63){   //Representable on 6 bits two complement
                string val = get_param_binary(addr);
                outfile << val << endl;
            }
            else if(addr<=2047){
                string val = get_constant_binary(addr);
                outfile << val.substr(0, 6) << " ";
                outfile << val.substr(6, 12) << endl;
            }

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
