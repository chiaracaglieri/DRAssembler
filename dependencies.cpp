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

int check(int r){
    if(regMap[r].inst==-1) return -1;
    else return regMap[r].until;
}

bool isConditional(string t){
    if(t=="EQ"      || \
       t=="LT"      || \
       t=="GT"      || \
       t=="LTE"     || \
       t=="GTE"     || \
       t=="EQ_0"    || \
       t=="LT_0"    || \
       t=="GT_0"    || \
       t=="LTE_0"   || \
       t=="GTE_0"   )
        return true;
    return false;
}
bool isMul(int i){
    if(code[i-1].type=="MUL" || \
       code[i-1].type=="MUL_I"  )
        return true;
    return false;
}

void printIUEU(int i1, int i2, int b){
    cout <<"Dip. IU-EU: "<<i1<<" => "<<i2<<"    Bolla da "<<b<<"t"<<endl;
}

void printEUEU(int i1, int i2, int b){
    cout <<"Dip. EU-EU: "<<i1<<" => "<<i2<<"    Bolla da "<<b<<"t"<<endl;
}

void printJump(int i){
    cout << "Salto preso: "<<i<<" Bolla da 1t"<<endl;
}

int main(int argc, char** argv){

    /*Preprocessing*/
    cout << "Start preprocessing..." << endl;
    int nstages=atoi(argv[2]);
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
    cout << endl;
    remove("tmp.drisc");
    cout << "Start analyzing..." << endl;

    int EU_until=0;
    int EUs_until=0;

    for(int i=0; i<code.size(); i++) {
        if(i==0) code[i].decode=1;
        int nregs = code[i].regs.size();
        string type = code[i].type;
        int num = code[i].number;
        int dec = code[i].decode;

        /**************************/

        if(code[i].regs.size()==0){
            /*END,NOP,GOTO*/
            if(type=="GOTO"){
                printJump(num);
                /*Aggiorno decode time dell'istr successiva*/
                if(i<code.size()-1) code[i+1].decode+=2;
            }
            else if(i<code.size()-1) code[i+1].decode=dec+1;
        }

        /**************************/

        if(code[i].regs.size()==1){
            /*One register to check*/
            int r0=code[i].regs[0];
            int blockingInst=regMap[r0].inst;
            int unt=check(r0);

            if(isConditional(type)){    /*IF*0*/
                if(unt!=-1 && unt>=dec){    /*Until indica l'ultimo stadio di occupazione della risorsa*/
                    /*Dipendenza IU-EU*/
                    printIUEU(blockingInst,num,unt+1-dec);
                    /*Aggiorno decode time dell'istr successiva*/
                    if(i<code.size()-1) code[i+1].decode=unt+1;
                }
                else if(i<code.size()-1) code[i+1].decode=dec+1;

                /*Eseguo Operazione*/
            }
            else{   /*MOVE_I,INCR,DECR,CLEAR*/
                if(unt!=-1 && unt>=dec+1 && isMul(blockingInst)){
                    printEUEU(blockingInst,num,unt-dec);
                    /*Aggiorno until e prenoto registro*/
                    regMap[r0].inst=num;
                    regMap[r0].until=unt+1;
                    /*La prox istruzione avrà cmomunque il decode allo stadio successivo ma dovrà tenere conto che la EU è occupata*/
                    EU_until=EUs_until+1;
                }
                else if(unt!=-1 && unt>=dec+1 && !isMul(blockingInst)){
                    /*Basta eseguire l'istruzione appena la EU è libera*/
                    regMap[r0].inst=num;
                    if(EU_until+1>=dec+1) EU_until++;
                    else EU_until=dec+1;
                    regMap[r0].until=EU_until;

                }
                else if(unt==-1 || unt<dec+1){
                    regMap[r0].inst=num;
                    if(EU_until+1>=dec+1) EU_until++;
                    else EU_until=dec+1;
                    regMap[r0].until=EU_until;
                }
                if(i<code.size()-1) code[i+1].decode=dec+1;

                /*Eseguo Operazione*/
            }
        }

        /**************************/

        if(code[i].regs.size()==2){
            /*Two registers to check*/
            int r0=code[i].regs[0];
            int r1=code[i].regs[1];
            int unt0=check(r0);
            int unt1=check(r1);
            int busyReg=-1;
            if(unt0==-1 && unt1==-1){
                /*No dependencies*/
                if(isAritm(type)){
                    /*Prenoto registro*/
                    regMap[r1].inst=num;
                    if(isMul(num)){
                        if(EU_until+1>=dec+1) EU_until++;
                        else EU_until=dec+1;
                        EUs_until=EU_until+nstages;
                        regMap[r1].until=EUs_until;
                    }
                    else{
                        if(EU_until+1>=dec+1) EU_until++;
                        else EU_until=dec+1;
                        regMap[r1].until=EU_until;
                    }
                }
                else if(type=="LOAD_I"){
                    /*Prenoto registro*/
                    regMap[r1].inst=num;
                    if(EU_until+1>=dec+2) EU_until++;
                    else EU_until=dec+2;
                    regMap[r1].until=EU_until;
                }
            }
            else if (unt0==-1) busyReg=r1;
            else if(unt1==-1) busyReg=r0;
            else{
                /*Both possibly cause dependency, choose worse*/
                if(unt0>=unt1) busyReg=r0;
                else busyReg=r1;
            }

            if(busyReg>0) { /*Esegue solo se c'è almeno una dipendenza*/
                int u=regMap[busyReg].until;
                int i=regMap[busyReg].inst;

                if (isConditional(type)) {    /*IF*/
                    if (u >= dec) {    /*Until indica l'ultimo stadio di occupazione della risorsa*/
                        /*Dipendenza IU-EU*/
                        printIUEU(i, num, u + 1 - dec);
                        /*Aggiorno decode time dell'istr successiva*/
                        if (i < code.size() - 1) code[i + 1].decode = u + 2;
                    } else if (i < code.size() - 1) code[i + 1].decode = dec + 1;

                    /*Eseguo Operazione*/
                } else if (isAritm(type)) {   /*MOVE,ADD_I,SUB_I,MUL_I*/
                    if ( u >= dec + 1 && isMul(i)) {
                        printEUEU(i, num, u - EU_until);
                        /*Aggiorno until e prenoto registro*/
                        regMap[r1].inst = num;
                        if(isMul(num)){
                            EU_until=EUs_until+1;
                            EUs_until=EUs_until+nstages+1;
                            regMap[r1].until = EUs_until;
                        }
                        else{
                            if (EU_until + 1 >= u + 1) EU_until++;
                            else EU_until = u + 1;
                            regMap[r1].until=EU_until;
                        }
                    } else if (u >= dec + 1 && !isMul(i)) {
                        /*Basta eseguire l'istruzione appena la EU è libera*/
                        regMap[r1].inst = num;
                        if (EU_until + 1 >= dec + 1) EU_until++;
                        else EU_until = dec + 1;
                        if (isMul(num)) {
                            EUs_until = EUs_until + nstages;
                            regMap[r1].until = EUs_until;
                        } else
                            regMap[r1].until = EU_until;
                    } else if ( u < dec + 1) {
                        regMap[r1].inst = num;
                        if (EU_until + 1 >= dec + 1) EU_until++;
                        else EU_until = dec + 1;
                        if (isMul(num)) {
                            EUs_until = EUs_until + nstages;
                            regMap[r1].until = EUs_until;
                        } else
                            regMap[r1].until = EU_until;
                    }

                }
                else if(type=="STORE_I"){
                    /*Dipendenza IU-EU*/
                    printIUEU(i, num, u + 1 - dec);
                    /*Aggiorno decode time dell'istr successiva*/
                    if (i < code.size() - 1) code[i + 1].decode = u + 2;
                }
                else if(type=="LOAD_I"){

                }
            }
            /*Eseguo operazione*/
            if (i < code.size() - 1) code[i + 1].decode = dec + 1;

        }
        if(code[i].regs.size()==3){

        }
    }

    return 0;
}
