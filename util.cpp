#include <unordered_map>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <bitset>
#include "util.h"
using namespace std;

std::string last_string;
std::string tmp;
int last_value;
std::unordered_map<std::string, int> symTable;
std::unordered_map<std::string, std::string > opTable;

int lc;
std::ofstream outfile ("assembled.txt");

bool find_symbol(std::string sym){
    if(symTable.find(sym)!=symTable.end())
        return true;
    else return false;
}

void insert_symbol(std::string sym, int lc){

    if(find_symbol(sym)) cout << "Symbol already" << "\n";
    else {symTable[sym]=lc; cout << "Symbol " << sym << " inserted" << "\n";}
}

/*AST Function Implementation*/

tree SynTree=NULL;

tree make_node(std::string type, int value, node* p1, node* p2, node* p3){
    tree new_node= new node;

    new_node->lc=lc;
    new_node->value=value;
    new_node->type=type;
    new_node->param1=p1;
    new_node->param2=p2;
    new_node->param3=p3;

    return new_node;
}

tree make_seq_node(std::vector<int> seq){
    tree new_node= new node;

    new_node->lc=lc;
    new_node->value=-1;
    new_node->seq=seq;
    new_node->type="SEQ";
    new_node->param1=NULL;
    new_node->param2=NULL;
    new_node->param3=NULL;

    return new_node;
}

void appendTree(node* node) {
    SynTree = node;
}

bitset<8> getOpcodeBinary(std::string s){
    stringstream ss;
    ss << hex << opTable[s];
    unsigned n;
    ss >> n;
    bitset<8> b(n);
    return b;
}

std::string getParamBinary(int v){
    bitset<6> b(v);
    return b.to_string();
}

void VisitTree(node* tmp){

    if(tmp==NULL) return;
    if(tmp->type!="INSTRUCTION" && tmp->type!="PROGRAM"){cout << "Im visiting the node: " << tmp->type << "\n";}
    if(tmp->type=="PROGRAM" || tmp->type=="INSTRUCTION"){
        VisitTree(tmp->param1);
        VisitTree(tmp->param2);
        VisitTree(tmp->param3);
    }
    else if(tmp->type=="loop") {
        VisitTree(tmp->param1);
    }
    else if(tmp->type=="ADD" || tmp->type=="SUB" || tmp->type=="MUL"){   //Expected 3 parameters
        if(tmp->type=="ADD"){
            if(tmp->param1->type=="REGISTER" && tmp->param2->type=="REGISTER" ){
                bitset<8> bin=getOpcodeBinary("ADD_RRR");
                outfile << bin.to_string() << " ";
            }
            else if(tmp->param1->type=="VALUE" && tmp->param2->type=="REGISTER" ){
                bitset<8> bin=getOpcodeBinary("ADD_IRR");
                outfile << bin.to_string() << " ";
            }
            else if(tmp->param1->type=="REGISTER" && tmp->param2->type=="VALUE" ){
                bitset<8> bin=getOpcodeBinary("ADD_RIR");
                outfile << bin.to_string() << " ";
            }
            else if(tmp->param1->type=="VALUE" && tmp->param2->type=="VALUE" ){
                bitset<8> bin=getOpcodeBinary("ADD_IIR");
                outfile << bin.to_string() << " ";
            }
        }

        if(tmp->type=="SUB"){
            if(tmp->param1->type=="REGISTER" && tmp->param2->type=="REGISTER" ){
                bitset<8> bin=getOpcodeBinary("SUB_RRR");
                outfile << bin.to_string() << " ";
            }
            else if(tmp->param1->type=="VALUE" && tmp->param2->type=="REGISTER" ){
                bitset<8> bin=getOpcodeBinary("SUB_IRR");
                outfile << bin.to_string() << " ";
            }
            else if(tmp->param1->type=="REGISTER" && tmp->param2->type=="VALUE" ){
                bitset<8> bin=getOpcodeBinary("SUB_RIR");
                outfile << bin.to_string() << " ";
            }
            else if(tmp->param1->type=="VALUE" && tmp->param2->type=="VALUE" ){
                bitset<8> bin=getOpcodeBinary("SUB_IIR");
                outfile << bin.to_string() << " ";
            }
        }

        if(tmp->type=="MUL"){
            if(tmp->param1->type=="REGISTER" && tmp->param2->type=="REGISTER" ){
                bitset<8> bin=getOpcodeBinary("MUL_RRR");
                outfile << bin.to_string() << " ";
            }
            else if(tmp->param1->type=="VALUE" && tmp->param2->type=="REGISTER" ){
                bitset<8> bin=getOpcodeBinary("MUL_IRR");
                outfile << bin.to_string() << " ";
            }
            else if(tmp->param1->type=="REGISTER" && tmp->param2->type=="VALUE" ){
                bitset<8> bin=getOpcodeBinary("MUL_RIR");
                outfile << bin.to_string() << " ";
            }
            else if(tmp->param1->type=="VALUE" && tmp->param2->type=="VALUE" ){
                bitset<8> bin=getOpcodeBinary("MUL_IIR");
                outfile << bin.to_string() << " ";
            }
        }

        std::string val=getParamBinary(tmp->param1->value);
        outfile << val << " ";
        val=getParamBinary(tmp->param2->value);
        outfile << val << " ";
        val=getParamBinary(tmp->param3->value);
        outfile << val<< std::endl;
        return;
    }
    else{
            bitset<8> bin=getOpcodeBinary(tmp->type);
            outfile << bin.to_string() << " ";
        }


    if(tmp->type=="MEMLOC" || tmp->type=="REGVAL"){ //Expected two integer values

        std::string val=getParamBinary(tmp->param1->value);
        outfile << val << " ";
        val=getParamBinary(tmp->param2->value);
        outfile << val<< std::endl;
        return;

    }

    if(tmp->type=="LOC" || tmp->type=="START" || tmp->type=="CLEAR" || tmp->type=="INCR"){ //Expected one integer value
        std::string val=getParamBinary(tmp->param1->value);
        outfile << val << std::endl;
        return;
    }

    if(tmp->type=="LOAD" || tmp->type=="STORE"){ //Expected three values
        std::string val=getParamBinary(tmp->param1->value);
        outfile << val << " ";
        val=getParamBinary(tmp->param2->value);
        outfile << val << " ";
        val=getParamBinary(tmp->param3->value);
        outfile << val<< std::endl;
        return;
    }


}

void loadOptable(){
    ifstream opcodes ("optable.txt");

    std::string word;
    while(opcodes >> word){
        std::string op=word;
        opcodes >> word;
        opTable[op]=word;
    }
}