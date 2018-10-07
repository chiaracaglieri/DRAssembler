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
    cout << "Start analyzing..." << endl;
    int nstages=0;
    for(int i=0; i<code.size(); i++){
        if(i>0) code[i].fetch=code[i-1].fetch+1;
        if(code[i].type!="NOP" && code[i].type!="END" && code[i].type!="GOTO"){
            if(code[i].type=="CLEAR" || code[i].type=="INCR" || code[i].type=="DECR"){
                nstages=3;
                /*One register to check*/
                if(regs[code[i].regs[0]].inst==-1 || \
                    regs[code[i].regs[0]].inst+regs[code[i].regs[0]].until<=code[i].number+1){
                    /*No dependency*/
                    regs[code[i].regs[0]].inst=code[i].number;
                    regs[code[i].regs[0]].until=nstages;
                }
                else{   /*Dependency*/

                    regs[code[i].regs[0]].inst=code[i].number;
                    regs[code[i].regs[0]].until=code[regs[code[i].regs[0]].inst].fetch+regs[code[i].regs[0]].until;
                    code[regs[code[i].regs[0]].inst].fetch=regs[code[i].regs[0]].until;
                }
            }
            else if(code[i].type=="ADD" || code[i].type=="SUB" || code[i].type=="LOAD" || code[i].type=="STORE" ){
                if(code[i].type=="LOAD") nstages=4;
                else if(code[i].type=="STORE") nstages=2;
                else nstages=3;
                if(code[i].regs.size()==2){
                    /*2 registers to check*/
                    if( (regs[code[i].regs[0]].inst==-1 && regs[code[i].regs[1]].inst==-1) \
                        || (code[regs[code[i].regs[0]].inst-1].fetch + regs[code[i].regs[0]].until<code[i].fetch && \
                        code[regs[code[i].regs[1]].inst-1].fetch + regs[code[i].regs[1]].until<code[i].fetch) ){
                        /*No dependencies*/
                            regs[code[i].regs[1]].inst=code[i].number;
                            regs[code[i].regs[1]].until=nstages;
                    }
                    else{   /*Dependency*/
                       if(regs[code[i].regs[0]].inst!=-1 && \
                       (code[regs[code[i].regs[0]].inst-1].fetch + regs[code[i].regs[0]].until)>=code[i].fetch){
                           cout << "Dip. IU-EU: " << regs[code[i].regs[0]].inst<<" => "<<code[i].number<<" Bolla da "<< code[regs[code[i].regs[0]].inst-1].fetch + regs[code[i].regs[0]].until-code[i].fetch-1 <<"t"<<endl;
                       }
                       if(regs[code[i].regs[1]].inst!=-1 && \
                          (code[regs[code[i].regs[1]].inst-1].fetch+regs[code[i].regs[1]].until)>=code[i].fetch){
                           cout << "Dip. IU-EU: " << regs[code[i].regs[1]].inst<<" => "<<code[i].number<<" Bolla da "<< code[regs[code[i].regs[1]].inst-1].fetch + regs[code[i].regs[1]].until-code[i].fetch-1 <<"t"<<endl;

                       }
                       if(code[regs[code[i].regs[0]].inst-1].fetch+regs[code[i].regs[0]].until >= \
                          code[regs[code[i].regs[1]].inst-1].fetch+regs[code[i].regs[1]].until){

                               regs[code[i].regs[1]].inst=code[i].number;
                               regs[code[i].regs[1]].until=code[regs[code[i].regs[0]].inst-1].fetch+regs[code[i].regs[0]].until;
                               code[i].fetch=code[regs[code[i].regs[0]].inst-1].fetch + regs[code[i].regs[0]].until;
                       }
                       else{
                           regs[code[i].regs[0]].inst=code[i].number;
                           regs[code[i].regs[0]].until=code[regs[code[i].regs[1]].inst-1].fetch+regs[code[i].regs[1]].until;
                           code[i].fetch=code[regs[code[i].regs[1]].inst-1].fetch + regs[code[i].regs[1]].until;
                       }
                    }
                }
                else if(code[i].regs.size()==3){
                    /*3 registers to check*/
                    if( (regs[code[i].regs[0]].inst==-1 && regs[code[i].regs[1]].inst==-1 && \
                        regs[code[i].regs[2]].inst==-1) || \
                        (code[regs[code[i].regs[0]].inst-1].fetch+regs[code[i].regs[0]].until<=code[i].fetch && \
                        code[regs[code[i].regs[1]].inst-1].fetch+regs[code[i].regs[1]].until<=code[i].fetch && \
                        code[regs[code[i].regs[2]].inst-1].fetch+regs[code[i].regs[2]].until<=code[i].fetch) ){
                        /*No dependencies*/
                        regs[code[i].regs[2]].inst=code[i].number;
                        regs[code[i].regs[2]].until=nstages;
                    }
                    else{   /*Dependency*/
                        if(regs[code[i].regs[0]].inst!=-1 && \
                       (code[regs[code[i].regs[0]].inst-1].fetch+regs[code[i].regs[0]].until)>code[i].fetch){
                            cout << "Dip. IU-EU: " << regs[code[i].regs[0]].inst<<" => "<<code[i].number<<" Bolla da "<< code[regs[code[i].regs[0]].inst-1].fetch + regs[code[i].regs[0]].until-code[i].fetch-1 <<"t"<<endl;;
                        }
                        if(regs[code[i].regs[1]].inst!=-1 && \
                          (code[regs[code[i].regs[1]].inst-1].fetch+regs[code[i].regs[1]].until)>code[i].fetch){
                            cout << "Dip. IU-EU: " << regs[code[i].regs[1]].inst<<" => "<<code[i].number<<" Bolla da "<< code[regs[code[i].regs[1]].inst-1].fetch + regs[code[i].regs[1]].until-code[i].fetch-1 <<"t"<<endl;;
                        }
                        if(regs[code[i].regs[2]].inst!=-1 && \
                          (code[regs[code[i].regs[2]].inst-1].fetch+regs[code[i].regs[2]].until)>code[i].fetch){
                            cout << "Dip. IU-EU: " << regs[code[i].regs[2]].inst<<" => "<<code[i].number<<" Bolla da "<< code[regs[code[i].regs[2]].inst-1].fetch + regs[code[i].regs[2]].until-code[i].fetch-1 <<"t"<<endl;;
                        }
                        if(code[regs[code[i].regs[0]].inst].fetch+regs[code[i].regs[0]].until >= \
                          code[regs[code[i].regs[1]].inst].fetch+regs[code[i].regs[1]].until && \
                          code[regs[code[i].regs[0]].inst].fetch+regs[code[i].regs[0]].until >= \
                          code[regs[code[i].regs[2]].inst].fetch+regs[code[i].regs[2]].until){

                            regs[code[i].regs[2]].inst=code[i].number;
                            regs[code[i].regs[2]].until=code[regs[code[i].regs[0]].inst-1].fetch+regs[code[i].regs[0]].until;
                            code[i].fetch=code[regs[code[i].regs[0]].inst-1].fetch + regs[code[i].regs[0]].until;
                        }
                        else if(code[regs[code[i].regs[1]].inst-1].fetch+regs[code[i].regs[1]].until >= \
                          code[regs[code[i].regs[0]].inst].fetch+regs[code[i].regs[0]].until && \
                          code[regs[code[i].regs[1]].inst].fetch+regs[code[i].regs[1]].until >= \
                          code[regs[code[i].regs[2]].inst].fetch+regs[code[i].regs[2]].until){

                            regs[code[i].regs[2]].inst = code[i].number;
                            regs[code[i].regs[2]].until = code[regs[code[i].regs[1]].inst-1].fetch+ regs[code[i].regs[1]].until;
                            code[i].fetch=code[regs[code[i].regs[1]].inst-1].fetch + regs[code[i].regs[1]].until;
                        }
                        else{
                            regs[code[i].regs[2]].inst=code[i].number;
                            regs[code[i].regs[2]].until=code[regs[code[i].regs[2]].inst-1].fetch+regs[code[i].regs[2]].until;
                            code[i].fetch=code[regs[code[i].regs[2]].inst-1].fetch + regs[code[i].regs[2]].until;
                        }
                    }
                }

            }
            else if(code[i].type=="IF"){
                if(code[i].regs.size()==1){
                    /*One register to check*/
                    if(regs[code[i].regs[0]].inst!=-1 && \
                    code[(regs[code[i].regs[0]].inst)-1].fetch+regs[code[i].regs[0]].until>=code[i].fetch){
                        /*Dependency*/
                        cout << "Dip. IU-EU: " << regs[code[i].regs[0]].inst<<" => "<<code[i].number << endl;
                    }
                }
                else{
                    /*2 registers to check*/
                    if(regs[code[i].regs[0]].inst!=-1 && code[(regs[code[i].regs[0]].inst)-1].fetch+regs[code[i].regs[0]].until>=code[i].fetch) {
                        cout << "Dip. IU-EU: " << regs[code[i].regs[0]].inst << " => " << code[i].number << " Bolla da " << code[(regs[code[i].regs[0]].inst)-1].fetch+regs[code[i].regs[0]].until-code[i].fetch-1<< "t" <<  endl;
                        code[i].fetch=code[(regs[code[i].regs[0]].inst)-1].fetch+regs[code[i].regs[0]].until;

                    }
                    if(regs[code[i].regs[1]].inst!=-1 && code[(regs[code[i].regs[0]].inst)-1].fetch+regs[code[i].regs[1]].until>=code[i].fetch)
                        cout << "Dip. IU-EU: " << regs[code[i].regs[1]].inst<<" => "<<code[i].number << endl;
                }

            }
            else if(code[i].type=="MOVE"){
                if(code[i].regs.size()==1){
                    /*One register to check*/
                    if(regs[code[i].regs[0]].inst==-1 || \
                    regs[code[i].regs[0]].inst+regs[code[i].regs[0]].until<=code[i].number+1){
                        /* No dependency, update*/
                        regs[code[i].regs[0]].inst=code[i].number;
                        regs[code[i].regs[0]].until=3;
                    }
                    else{
                        /*Dependency*/
                        cout << "Dip. IU-EU: " << regs[code[i].regs[0]].inst<<" => "<<code[i].number<< endl;
                        regs[code[i].regs[0]].inst=code[i].number;
                        regs[code[i].regs[0]].until=code[regs[code[i].regs[0]].inst].fetch+regs[code[i].regs[0]].until;
                        code[regs[code[i].regs[0]].inst].fetch=regs[code[i].regs[0]].until;
                    }
                }
                else{
                    /*2 registers to check*/
                    if( (regs[code[i].regs[0]].inst==-1 && regs[code[i].regs[1]].inst==-1) || \
                    (regs[code[i].regs[0]].inst+regs[code[i].regs[0]].until<=code[i].number+1 && \
                    regs[code[i].regs[1]].inst+regs[code[i].regs[1]].until<=code[i].number+1) ){
                        /* No dependency, update*/
                        regs[code[i].regs[1]].inst=code[i].number;
                        regs[code[i].regs[1]].until=3;
                    }
                    else{
                        if(regs[code[i].regs[0]].inst!=-1 && \
                       (regs[code[i].regs[0]].inst+regs[code[i].regs[0]].until)>=code[i].number+1){
                            cout << "Dip. IU-EU: " << regs[code[i].regs[0]].inst<<" => "<<code[i].number<<endl;
                        }
                        if(regs[code[i].regs[1]].inst!=-1 && \
                          (regs[code[i].regs[1]].inst+regs[code[i].regs[1]].until)>=code[i].number+1){
                            cout << "Dip. IU-EU: " << regs[code[i].regs[1]].inst<<" => "<<code[i].number<<endl;
                        }
                        if(regs[code[i].regs[0]].inst+regs[code[i].regs[0]].until >= \
                          regs[code[i].regs[1]].inst+regs[code[i].regs[1]].until){
                            //for(int k=0; k<2; k++){
                            regs[code[i].regs[1]].inst=code[i].number;
                            regs[code[i].regs[1]].until=code[regs[code[i].regs[0]].inst].fetch+regs[code[i].regs[0]].until;
                            code[regs[code[i].regs[0]].inst].fetch=regs[code[i].regs[1]].until;
                            //}
                        }
                        else{
                            regs[code[i].regs[1]].inst=code[i].number;
                            regs[code[i].regs[1]].until=code[regs[code[i].regs[1]].inst].fetch+regs[code[i].regs[1]].until;
                            code[regs[code[i].regs[1]].inst].fetch=regs[code[i].regs[1]].until;
                        }

                    }
                }
            }

        }
    }
    return 0;
}
