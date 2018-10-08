#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <map>
#include "asm1.tab.h"
#include "util.h"
#include "util_dip.h"

extern FILE* yyin;


int main(int argc, char** argv){

    /*Preprocessing*/
    cout << "Start preprocessing..." << endl;
    ifstream file(argv[1]);
    /*Check file type*/
    string filename=argv[1];
    if(filename.substr(filename.find_last_of(".") + 1) != "drisc") {
        cerr << "Error,  \""<< filename << "\" is not of type .drisc..." << endl;
        exit(EXIT_FAILURE);
    }
    ofstream tmp1("tmp.drisc");

    tmp1 << file.rdbuf();
    tmp1.close();

    ifstream tmp2("tmp.drisc");
    string str;
    while (getline(tmp2,str)){
        if(str[0]=='#'){
            vector<string> v;
            istringstream buf(str);
            for(string word; buf >> word; )
                v.push_back(word);
            substitute("tmp.drisc", v[1],v[2]);
        }
    }

    yyin=fopen("tmp.drisc","r");
    cout << "Start parsing..." << endl;
    yyparse();
    remove("tmp.drisc");
    cout << "Start analyzing..." << endl;

    for(int i=0; i<code.size(); i++){
        if(i>0) code[i].fetch=code[i-1].fetch+1;

        string type=code[i].type;
        int n=code[i].number;

        if(code[i].regs.size()==0){
            /*No registers to check*/
            if(type=="GOTO") cout << "Salto preso: "<< code[i].number << "     Bolla da 1t" << endl;
        }
        else if(code[i].regs.size()==1){
            /*One register to check*/
            if(regMap[code[i].regs[0]].inst!=-1 && code[regMap[code[i].regs[0]].inst-1].fetch+regMap[code[i].regs[0]].until>=code[i].fetch+1){
                /*Dependency*/
                cout << "Dip. IU-EU: " << regMap[code[i].regs[0]].inst << " => " << code[i].number;
                cout << " Bolla da  " << code[regMap[code[i].regs[0]].inst-1].fetch + regMap[code[i].regs[0]].until - code[i].fetch << "t" <<endl;
                code[i].fetch=code[regMap[code[i].regs[0]].inst-1].fetch + regMap[code[i].regs[0]].until;
            }
            if(type=="EQ_0" || type=="LT_0" || type=="GT_0" || type=="GTE_0" || type=="LTE_0"){
                /*Apply operation*/
                if(type=="EQ_0" && regMap[code[i].regs[0]].value==0){
                    cout << "Salto preso: "<< code[i].number << "     Bolla da 1t" << endl;
                    code[i].fetch++;
                }
                else if(type=="LT_0" && regMap[code[i].regs[0]].value<0){
                    cout << "Salto preso: "<< code[i].number << "     Bolla da 1t" << endl;
                }
                else if(type=="GT_0" && regMap[code[i].regs[0]].value>0){
                    cout << "Salto preso: "<< code[i].number << "     Bolla da 1t" << endl;
                }
                else if(type=="GTE_0" && regMap[code[i].regs[0]].value>=0){
                    cout << "Salto preso: "<< code[i].number << "     Bolla da 1t" << endl;
                }
                else if(type=="LTE_0" && regMap[code[i].regs[0]].value<=0){
                    cout << "Salto preso: "<< code[i].number << "     Bolla da 1t" << endl;
                }
            }
            else{   /* CLEAR, INCR, DECR, MOVE_I */
                /*Claim register*/
                regMap[code[i].regs[0]].inst=n;
                regMap[code[i].regs[0]].until=2;

                /*Apply operation*/
                if(type=="MOVE_I")  regMap[code[i].regs[0]].value=code[i].imm;
                else if(type=="CLEAR") regMap[code[i].regs[0]].value=0;
                else if(type=="INCR") regMap[code[i].regs[0]].value++;
                else regMap[code[i].regs[0]].value--;
            }

        }
        else if(code[i].regs.size()==2){

            /*Two registers to check*/
            if( (regMap[code[i].regs[0]].inst!=-1 && code[regMap[code[i].regs[0]].inst-1].fetch+regMap[code[i].regs[0]].until>=code[i].fetch+1) || \
                (regMap[code[i].regs[1]].inst!=-1 && code[regMap[code[i].regs[1]].inst-1].fetch+regMap[code[i].regs[1]].until>=code[i].fetch+1) ){
                /*Dependency*/
                if((regMap[code[i].regs[0]].inst!=-1 && code[regMap[code[i].regs[0]].inst-1].fetch+regMap[code[i].regs[0]].until>=code[i].fetch+1) && \
                (regMap[code[i].regs[1]].inst!=-1 && code[regMap[code[i].regs[1]].inst-1].fetch+regMap[code[i].regs[1]].until>=code[i].fetch+1)) {
                    /*Both registers cause dependency, consider the one that'll be available for last*/
                    cout << "Dip. IU-EU: " << regMap[code[i].regs[0]].inst << " => " << code[i].number;
                    cout << " Bolla da  " << code[regMap[code[i].regs[0]].inst-1].fetch + regMap[code[i].regs[0]].until - code[i].fetch << "t" <<endl;
                    cout << "Dip. IU-EU: " << regMap[code[i].regs[1]].inst << " => " << code[i].number;
                    cout << " Bolla da  " << code[regMap[code[i].regs[1]].inst-1].fetch + regMap[code[i].regs[1]].until - code[i].fetch << "t" <<endl;

                    code[i].fetch=max(code[regMap[code[i].regs[0]].inst-1].fetch + regMap[code[i].regs[0]].until,code[regMap[code[i].regs[1]].inst-1].fetch+regMap[code[i].regs[1]].until);

                }
                else if(regMap[code[i].regs[0]].inst!=-1 && code[regMap[code[i].regs[0]].inst-1].fetch+regMap[code[i].regs[0]].until>=code[i].fetch+1){
                    /*First register's fault*/
                    cout << "Dip. IU-EU: " << regMap[code[i].regs[0]].inst << " => " << code[i].number;
                    cout << " Bolla da  " << code[regMap[code[i].regs[0]].inst-1].fetch + regMap[code[i].regs[0]].until - code[i].fetch << "t" <<endl;
                    code[i].fetch=code[regMap[code[i].regs[0]].inst-1].fetch + regMap[code[i].regs[0]].until;
                }
                else if(regMap[code[i].regs[1]].inst!=-1 && code[regMap[code[i].regs[1]].inst-1].fetch+regMap[code[i].regs[1]].until>=code[i].fetch+1){
                    /*Second register's fault*/
                    cout << "Dip. IU-EU: " << regMap[code[i].regs[1]].inst << " => " << code[i].number;
                    cout << " Bolla da  " << code[regMap[code[i].regs[1]].inst-1].fetch + regMap[code[i].regs[1]].until - code[i].fetch << "t" <<endl;
                    code[i].fetch=code[regMap[code[i].regs[1]].inst-1].fetch + regMap[code[i].regs[1]].until;
                }
            }

            if(type=="ADD_I" || type=="SUB_I" || type=="LOAD_I" || type=="STORE_I" || type=="MOVE"){
                /*Claim register*/
                if(type!="STORE_I") regMap[code[i].regs[1]].inst=n;
                if(type=="LOAD_I") regMap[code[i].regs[1]].until=3;
                else regMap[code[i].regs[1]].until=2;

                /*Apply operation*/
                if(type=="ADD_I") regMap[code[i].regs[1]].value=regMap[code[i].regs[0]].value+code[i].imm;
                else if(type=="SUB_I") regMap[code[i].regs[1]].value=regMap[code[i].regs[0]].value-code[i].imm;
                else if(type=="LOAD_I") regMap[code[i].regs[1]].value=memMap[regMap[code[i].regs[0]].value]+memMap[code[i].imm];
                else if(type=="STORE_I") memMap[regMap[code[i].regs[0]].value+code[i].imm]=regMap[code[i].regs[1]].value;
                else if(type=="MOVE") regMap[code[i].regs[1]].value=regMap[code[i].regs[0]].value;
            }
            if(type=="EQ" || type=="GT" || type=="LT" || type=="GTE" || type=="LTE"){
                /*Apply operation*/
                if(type=="EQ" && regMap[code[i].regs[0]].value==regMap[code[i].regs[1]].value){
                    cout << "Salto preso: "<< code[i].number << "     Bolla da 1t" << endl;
                    code[i].fetch++;
                }
                else if(type=="LT" && regMap[code[i].regs[0]].value<regMap[code[i].regs[1]].value){
                    cout << "Salto preso: "<< code[i].number << "     Bolla da 1t" << endl;
                }
                else if(type=="GT" && regMap[code[i].regs[0]].value>regMap[code[i].regs[1]].value){
                    cout << "Salto preso: "<< code[i].number << "     Bolla da 1t" << endl;
                }
                else if(type=="GTE" && regMap[code[i].regs[0]].value>=regMap[code[i].regs[1]].value){
                    cout << "Salto preso: "<< code[i].number << "     Bolla da 1t" << endl;
                }
                else if(type=="LTE" && regMap[code[i].regs[0]].value<=regMap[code[i].regs[1]].value){
                    cout << "Salto preso: "<< code[i].number << "     Bolla da 1t" << endl;
                }
            }

        }
        else if(code[i].regs.size()==3){
            /*Three registers to check*/
            if( (regMap[code[i].regs[0]].inst!=-1 && code[regMap[code[i].regs[0]].inst-1].fetch+regMap[code[i].regs[0]].until>=code[i].fetch+1) || \
                (regMap[code[i].regs[1]].inst!=-1 && code[regMap[code[i].regs[1]].inst-1].fetch+regMap[code[i].regs[1]].until>=code[i].fetch+1) || \
                (regMap[code[i].regs[2]].inst!=-1 && code[regMap[code[i].regs[2]].inst-1].fetch+regMap[code[i].regs[2]].until>=code[i].fetch+1)){

                /*Dependency*/
                if((regMap[code[i].regs[0]].inst!=-1 && code[regMap[code[i].regs[0]].inst-1].fetch+regMap[code[i].regs[0]].until>=code[i].fetch+1) && \
                (regMap[code[i].regs[1]].inst!=-1 && code[regMap[code[i].regs[1]].inst-1].fetch+regMap[code[i].regs[1]].until>=code[i].fetch+1) && \
                (regMap[code[i].regs[2]].inst!=-1 && code[regMap[code[i].regs[2]].inst-1].fetch+regMap[code[i].regs[2]].until>=code[i].fetch+1) ) {
                    /*All three registers cause dependency, consider the one that'll be available for last*/
                    cout << "Dip. IU-EU: " << regMap[code[i].regs[0]].inst << " => " << code[i].number;
                    cout << " Bolla da  " << code[regMap[code[i].regs[0]].inst-1].fetch + regMap[code[i].regs[0]].until - code[i].fetch << "t" <<endl;
                    cout << "Dip. IU-EU: " << regMap[code[i].regs[1]].inst << " => " << code[i].number;
                    cout << " Bolla da  " << code[regMap[code[i].regs[1]].inst-1].fetch + regMap[code[i].regs[1]].until - code[i].fetch << "t" <<endl;
                    cout << "Dip. IU-EU: " << regMap[code[i].regs[2]].inst << " => " << code[i].number;
                    cout << " Bolla da  " << code[regMap[code[i].regs[2]].inst-1].fetch + regMap[code[i].regs[2]].until - code[i].fetch << "t" <<endl;

                    code[i].fetch=max(max(code[regMap[code[i].regs[0]].inst-1].fetch + regMap[code[i].regs[0]].until,code[regMap[code[i].regs[1]].inst-1].fetch+regMap[code[i].regs[1]].until), \
                                          code[regMap[code[i].regs[2]].inst-1].fetch + regMap[code[i].regs[2]].until);

                }
                else if((regMap[code[i].regs[0]].inst!=-1 && code[regMap[code[i].regs[0]].inst-1].fetch+regMap[code[i].regs[0]].until>=code[i].fetch+1) && \
                (regMap[code[i].regs[1]].inst!=-1 && code[regMap[code[i].regs[1]].inst-1].fetch+regMap[code[i].regs[1]].until>=code[i].fetch+1)) {
                    /*First two registers cause dependency, consider the one that'll be available for last*/
                    cout << "Dip. IU-EU: " << regMap[code[i].regs[0]].inst << " => " << code[i].number;
                    cout << " Bolla da  " << code[regMap[code[i].regs[0]].inst-1].fetch + regMap[code[i].regs[0]].until - code[i].fetch << "t" <<endl;
                    cout << "Dip. IU-EU: " << regMap[code[i].regs[1]].inst << " => " << code[i].number;
                    cout << " Bolla da  " << code[regMap[code[i].regs[1]].inst-1].fetch + regMap[code[i].regs[1]].until - code[i].fetch << "t" <<endl;

                    code[i].fetch=max(code[regMap[code[i].regs[0]].inst-1].fetch + regMap[code[i].regs[0]].until,code[regMap[code[i].regs[1]].inst-1].fetch+regMap[code[i].regs[1]].until);

                }
                else if((regMap[code[i].regs[1]].inst!=-1 && code[regMap[code[i].regs[1]].inst-1].fetch+regMap[code[i].regs[1]].until>=code[i].fetch+1) && \
                (regMap[code[i].regs[2]].inst!=-1 && code[regMap[code[i].regs[2]].inst-1].fetch+regMap[code[i].regs[2]].until>=code[i].fetch+1)) {
                    /*Last two registers cause dependency, consider the one that'll be available for last*/
                    cout << "Dip. IU-EU: " << regMap[code[i].regs[1]].inst << " => " << code[i].number;
                    cout << " Bolla da  " << code[regMap[code[i].regs[1]].inst-1].fetch + regMap[code[i].regs[1]].until - code[i].fetch << "t" <<endl;
                    cout << "Dip. IU-EU: " << regMap[code[i].regs[2]].inst << " => " << code[i].number;
                    cout << " Bolla da  " << code[regMap[code[i].regs[2]].inst-1].fetch + regMap[code[i].regs[2]].until - code[i].fetch << "t" <<endl;

                    code[i].fetch=max(code[regMap[code[i].regs[1]].inst-1].fetch + regMap[code[i].regs[1]].until,code[regMap[code[i].regs[2]].inst-1].fetch+regMap[code[i].regs[2]].until);

                }
                else if((regMap[code[i].regs[0]].inst!=-1 && code[regMap[code[i].regs[0]].inst-1].fetch+regMap[code[i].regs[0]].until>=code[i].fetch+1) && \
                (regMap[code[i].regs[2]].inst!=-1 && code[regMap[code[i].regs[2]].inst-1].fetch+regMap[code[i].regs[2]].until>=code[i].fetch+1)) {
                    /*First and last registers cause dependency, consider the one that'll be available for last*/
                    cout << "Dip. IU-EU: " << regMap[code[i].regs[0]].inst << " => " << code[i].number;
                    cout << " Bolla da  " << code[regMap[code[i].regs[0]].inst-1].fetch + regMap[code[i].regs[0]].until - code[i].fetch << "t" <<endl;
                    cout << "Dip. IU-EU: " << regMap[code[i].regs[2]].inst << " => " << code[i].number;
                    cout << " Bolla da  " << code[regMap[code[i].regs[2]].inst-1].fetch + regMap[code[i].regs[2]].until - code[i].fetch << "t" <<endl;

                    code[i].fetch=max(code[regMap[code[i].regs[0]].inst-1].fetch + regMap[code[i].regs[0]].until,code[regMap[code[i].regs[2]].inst-1].fetch+regMap[code[i].regs[2]].until);

                }
                else if(regMap[code[i].regs[0]].inst!=-1 && code[regMap[code[i].regs[0]].inst-1].fetch+regMap[code[i].regs[0]].until>=code[i].fetch+1){
                    /*First register's fault*/
                    cout << "Dip. IU-EU: " << regMap[code[i].regs[0]].inst << " => " << code[i].number;
                    cout << " Bolla da  " << code[regMap[code[i].regs[0]].inst-1].fetch + regMap[code[i].regs[0]].until - code[i].fetch << "t" <<endl;
                    code[i].fetch=code[regMap[code[i].regs[0]].inst-1].fetch + regMap[code[i].regs[0]].until;
                }
                else if(regMap[code[i].regs[1]].inst!=-1 && code[regMap[code[i].regs[1]].inst-1].fetch+regMap[code[i].regs[1]].until>=code[i].fetch+1){
                    /*Second register's fault*/
                    cout << "Dip. IU-EU: " << regMap[code[i].regs[1]].inst << " => " << code[i].number;
                    cout << " Bolla da  " << code[regMap[code[i].regs[1]].inst-1].fetch + regMap[code[i].regs[1]].until - code[i].fetch << "t" <<endl;
                    code[i].fetch=code[regMap[code[i].regs[1]].inst-1].fetch + regMap[code[i].regs[1]].until;
                }
                else if(regMap[code[i].regs[2]].inst!=-1 && code[regMap[code[i].regs[2]].inst-1].fetch+regMap[code[i].regs[2]].until>=code[i].fetch+1){
                    /*Third register's fault*/
                    cout << "Dip. IU-EU: " << regMap[code[i].regs[2]].inst << " => " << code[i].number;
                    cout << " Bolla da  " << code[regMap[code[i].regs[2]].inst-1].fetch + regMap[code[i].regs[2]].until - code[i].fetch << "t" <<endl;
                    code[i].fetch=code[regMap[code[i].regs[2]].inst-1].fetch + regMap[code[i].regs[2]].until;
                }
            }
            if(type=="ADD" || type=="SUB" || type=="STORE" || type=="LOAD"){
                /*Claim register*/
                if(type!="STORE") regMap[code[i].regs[2]].inst=n;
                if(type=="LOAD") regMap[code[i].regs[2]].until=3;
                else regMap[code[i].regs[2]].until=2;

                /*Apply operation*/
                if(type=="ADD") regMap[code[i].regs[2]].value=regMap[code[i].regs[0]].value+regMap[code[i].regs[1]].value;
                else if(type=="SUB") regMap[code[i].regs[2]].value=regMap[code[i].regs[0]].value-regMap[code[i].regs[1]].value;
                else if(type=="LOAD") regMap[code[i].regs[2]].value=memMap[regMap[code[i].regs[0]].value]+memMap[regMap[code[i].regs[1]].value];
                else if(type=="STORE") memMap[regMap[code[i].regs[0]].value+regMap[code[i].regs[1]].value]=regMap[code[i].regs[2]].value;
            }
        }
    }
    return 0;
}
