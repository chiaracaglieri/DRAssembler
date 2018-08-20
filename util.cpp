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
std::vector<int> v;
int last_value;
std::unordered_map<std::string, int> symTable;
std::unordered_map<std::string, std::string > opTable;

int lc;
std::ofstream outfile ("assembled.txt");

int find_symbol(std::string sym){
    if(symTable.find(sym)!=symTable.end())
        return symTable[sym];
    else return -1;
}

void insert_symbol(std::string sym, int lc){

    if(find_symbol(sym)!=-1) cout << "Symbol already" << "\n";
    else {symTable[sym]=lc; cout << "Symbol " << sym << " inserted at position " << lc << "\n";}
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
    bitset<16> b(v);
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
    else if(tmp->type=="EQ" || tmp->type=="LT" || tmp->type=="GT"){   //Expected 3 parameters, last one is a label
        if(tmp->type=="EQ"){
            if(tmp->param1->type=="REGISTER" && tmp->param2->type=="REGISTER" ){
                bitset<8> bin=getOpcodeBinary("EQ_RR");
                outfile << bin.to_string() << " ";
            }
            else if(tmp->param1->type=="VALUE" && tmp->param2->type=="REGISTER" ){
                bitset<8> bin=getOpcodeBinary("EQ_IR");
                outfile << bin.to_string() << " ";
            }
            else if(tmp->param1->type=="REGISTER" && tmp->param2->type=="VALUE" ){
                bitset<8> bin=getOpcodeBinary("EQ_RI");
                outfile << bin.to_string() << " ";
            }
            else if(tmp->param1->type=="VALUE" && tmp->param2->type=="VALUE" ){
                bitset<8> bin=getOpcodeBinary("EQ_II");
                outfile << bin.to_string() << " ";
            }
        }

        if(tmp->type=="LT"){
            if(tmp->param1->type=="REGISTER" && tmp->param2->type=="REGISTER" ){
                bitset<8> bin=getOpcodeBinary("LT_RR");
                outfile << bin.to_string() << " ";
            }
            else if(tmp->param1->type=="VALUE" && tmp->param2->type=="REGISTER" ){
                bitset<8> bin=getOpcodeBinary("LT_IR");
                outfile << bin.to_string() << " ";
            }
            else if(tmp->param1->type=="REGISTER" && tmp->param2->type=="VALUE" ){
                bitset<8> bin=getOpcodeBinary("LT_RI");
                outfile << bin.to_string() << " ";
            }
            else if(tmp->param1->type=="VALUE" && tmp->param2->type=="VALUE" ){
                bitset<8> bin=getOpcodeBinary("LT_II");
                outfile << bin.to_string() << " ";
            }
        }

        if(tmp->type=="GT"){
            if(tmp->param1->type=="REGISTER" && tmp->param2->type=="REGISTER" ){
                bitset<8> bin=getOpcodeBinary("GT_RR");
                outfile << bin.to_string() << " ";
            }
            else if(tmp->param1->type=="VALUE" && tmp->param2->type=="REGISTER" ){
                bitset<8> bin=getOpcodeBinary("GT_IR");
                outfile << bin.to_string() << " ";
            }
            else if(tmp->param1->type=="REGISTER" && tmp->param2->type=="VALUE" ){
                bitset<8> bin=getOpcodeBinary("GT_RI");
                outfile << bin.to_string() << " ";
            }
            else if(tmp->param1->type=="VALUE" && tmp->param2->type=="VALUE" ){
                bitset<8> bin=getOpcodeBinary("GT_II");
                outfile << bin.to_string() << " ";
            }
        }

        std::string val=getParamBinary(tmp->param1->value);
        outfile << val << " ";
        val=getParamBinary(tmp->param2->value);
        outfile << val << " ";
        int addr=find_symbol(tmp->param3->type);
        if(addr==-1) cout << "Error, symbol not in Symbol Table!" <<"\n";
        else {
            val = getParamBinary(addr);
            outfile << val << std::endl;
        }
        return;
    }
    else if(tmp->type=="LABEL"){
        int addr=find_symbol(tmp->param1->type);
        if(addr==-1) cout << "Error, symbol not in Symbol Table!" <<"\n";
        else VisitTree(tmp->param2);
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
    if(tmp->type=="MEMLOCS"){   //Multiple parameters
        std::string val=getParamBinary(tmp->param1->value);
        outfile << val << " ";


        for(int i=0; i<v.size(); i++){
            val=getParamBinary(v[i]);
            outfile << val << " ";
        }

        outfile << std::endl;
    }

    if(tmp->type=="GOTO"){
        int addr=find_symbol(tmp->param1->type);
        if(addr==-1) cout << "Error, symbol not in Symbol Table!" <<"\n";
        else {
            std::string val = getParamBinary(addr);
            outfile << val << std::endl;
        }
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