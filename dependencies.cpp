/**
 * @file dependencies.cpp
 * @author Chiara Caglieri
 * @brief Main file for the analyzer
 */

#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <vector>
#include <unordered_map>
#include <map>
#include "asm.tab.h"
#include "parse_util.h"
#include "dep_util.h"

extern FILE* yyin;     /**<Pointer to the file containing the data to parse*/

string divider="";     /**<Divider to better format the output*/
vector<int> already;   /**<Keeps track of the instructions that have been analyzed already*/

/** \function check
  * @brief Checks whether the register is busy or not
  * @param r the register to check
  * @param m the table to search in
  * @return m[r].until the last stage at which the register will be in use
  *         -1 if the register is not in use
  */
int check(int r, map<int,reg> m){
    if(m[r].inst==-1) return -1;
    else return m[r].until;
}

/** \function isConditional
  * @brief determines whether the operation is of conditional type
  * @param op the operation to check
  * @return true if the op. is of conditional type
  *         false otherwise
 */
bool isConditional(string t){
    if(t=="EQ"      || \
       t=="LT"      || \
       t=="GT"      || \
       t=="LTE"     || \
       t=="GTE"     || \
       t=="NEQ"     || \
       t=="EQ_0"    || \
       t=="LT_0"    || \
       t=="GT_0"    || \
       t=="LTE_0"   || \
       t=="GTE_0"   || \
       t=="NEQ_0")
        return true;
    return false;
}

/** \function isLong
  * @brief determines whether the operation is a long arithmetic
  * @param op the operation to check
  * @return true if the op. is a long arithmetic
  *         false otherwise
 */
bool isLong(int i){
    if(prog[i-1].type=="MUL" || \
       prog[i-1].type=="MUL_I" || \
       prog[i-1].type=="DIV" || \
       prog[i-1].type=="DIV_I" || \
       prog[i-1].type=="MOD" || \
       prog[i-1].type=="MOD_I")
        return true;
    return false;
}

/** \function printIUEU
  * @brief Formats the output for a IU-EU dependency
  * @param i1 the index of the instruction causing the dependency
  * @param i2 the index of the instruction affected by the dependency
  * @param b the length of the resulting gap
  * @param reg the register causing the dependency
  */
void printIUEU(int i1, int i2, int b, int reg){
    cout <<divider<<"Dip. EU-IU: "<<i1<<" => "<<i2<<" a causa di "<<checkSym(reg)<<"    Bolla da "<<b<<"t"<<endl;
}

/** \function printEUEU
  * @brief Formats the output for a EU-EU dependency
  * @param i1 the index of the instruction causing the dependency
  * @param i2 the index of the instruction affected by the dependency
  * @param b the length of the resulting gap
  * @param reg the register causing the dependency
  */
void printEUEU(int i1, int i2, int b, int reg){
    cout <<divider<<"Dip. EU-EU: "<<i1<<" => "<<i2<<" a causa di "<<checkSym(reg)<<"    Bolla da "<<b<<"t"<<endl;
}

/** \function printJump
  * @brief Formats the output for a jump instruction
  * @param i the index of the instruction causing the jump
  */
void printJump(int i){
    cout <<divider<<i<< ": Salto preso   Bolla da 1t"<<endl;
}

/** \function analyzer
  * @brief Checks for logic dependencies among instructions
  * @param start the first instruction to examine
  * @param nstages the number of stages for the EU slave
  * @param EU_until the stage at which the EU is last in use
  * @param EUs_until the stage at which the EU slave is last in use
  * @param termination if set causes the analysis to stop as soon as no dependency is found
  * @param code the vector containing thew instructions
  * @param regMap the table containing registers and information on their state
  */
void analyzer(int start,int stop,int nstages, int EU_until, int EUs_until, int termination,vector<instruction> code, map<int,reg> regMap){

    for(int i=start; i<code.size(); i++) {
        if(find(already.begin(),already.end(),i)!=already.end()) break;
        if(i==0 && already.empty()) code[i].decode=1;
        int nregs = code[i].regs.size();    //Number of registers to check
        string type = code[i].type;
        int num = code[i].number;
        int dec = code[i].decode;
        /*No registers to check*/

        if(nregs==0){  //END,NOP,GOTO
            if(type=="GOTO_I"){
                printJump(num);
                code[symbolMap[code[i].label]-1].decode+=2; //Update decode of next instr.
                //if( find(already.begin(), already.end(), i)!= already.end() ) break;
                i-=symbolMap[code[i].label]-1;
            }
            else if(i<code.size()-1){
                code[i+1].decode=dec+1;
                //if( find(already.begin(), already.end(), i)!= already.end() ) break;
            }
        }
        /*One register to check*/
        if(nregs==1){
            int r0=code[i].regs[0];
            int blockingInst=regMap[r0].inst;
            int unt=check(r0,regMap);

            if(isConditional(type)){    //IF0
                if(unt!=-1 && unt>=dec){ //EU-IU Dependency
                    printIUEU(blockingInst,num,unt+1-dec,r0);
                    if(i<code.size()-1) code[i+1].decode=unt+1;
                }
                else if(i<code.size()-1){
                    code[i+1].decode=dec+1;
                    //if( find(already.begin(), already.end(), i)!= already.end() ) break;
                }

                printJump(num);
                int tmp = code[symbolMap[code[i].label]-1].decode;
                code[symbolMap[code[i].label]-1].decode=code[i+1].decode;
                string old_div=divider;
                divider.append("\t");
                analyzer(symbolMap[code[i].label]-1,i,nstages,EU_until,EUs_until,0,code,regMap);
                cout<<old_div<<num<<": Salto non preso"<<endl;
                code[symbolMap[code[i].label]-1].decode=tmp; //Restore decode time of instruction after jump

            }
            else{   //MOVE_I,INCR,DECR,CLEAR
                if(unt!=-1 && unt>=dec+1 && isLong(blockingInst)){ //EU-EU Dependency
                    printEUEU(blockingInst,num,unt-dec,r0);
                    regMap[r0].inst=num;    //Claim register
                    EU_until=EUs_until+1;
                    regMap[r0].until=EU_until;
                }
                else if(unt!=-1 && unt>=dec+1 && !isLong(blockingInst)){ //Wait for EU to be free, then proceed
                    regMap[r0].inst=num;    //Claim register
                    if(EU_until>=dec+1) EU_until++;
                    else EU_until=dec+1;
                    regMap[r0].until=EU_until;
                    //if( find(already.begin(), already.end(), i)!= already.end() ) break;
                }
                else if(unt==-1 || unt<dec+1){
                    regMap[r0].inst=num;    //Claim register
                    if(EU_until>=dec+1) EU_until++;
                    else EU_until=dec+1;
                    regMap[r0].until=EU_until;
                    //if( find(already.begin(), already.end(), i)!= already.end() ) break;
                }
                if(i<code.size()-1) code[i+1].decode=dec+1; //Update decode of next instr.
            }
        }
        /*Two registers to check*/
        if(nregs==2){
            int r0=code[i].regs[0];
            int r1=code[i].regs[1];
            int unt0=check(r0,regMap);
            int unt1=check(r1,regMap);
            int busyReg=-1;
            if(unt0==-1 && unt1==-1){ //No dependencies
                if(isAritm(type)) {  //ADD_I,SUB_I,MUL_I,MOVE, DIV_I,MOD_I
                    regMap[r1].inst = num;    //Claim register
                    if (EU_until >= dec + 1) EU_until++;
                    else EU_until = dec + 1;
                    regMap[r1].until = EU_until;
                    if(isLong(num)) {
                        EUs_until = EU_until + nstages;
                        regMap[r1].until = EUs_until;
                    }

                }
                else if(type=="STORE_I"){
                    if(EU_until>=dec+1) EU_until++;
                    else EU_until=dec+1;
                    regMap[r1].until=EU_until;
                }
                else if(type=="LOAD_I"){
                    regMap[r1].inst=num;
                    if(EU_until>=dec+2) EU_until++;
                    else EU_until=dec+2;
                    regMap[r1].until=EU_until;
                }
                else if(isConditional(type)){   //IF
                    printJump(num);
                    int tmp = code[symbolMap[code[i].label]-1].decode;
                    code[symbolMap[code[i].label]-1].decode=code[i+1].decode;
                    string old_div=divider;
                    divider.append("\t");
                    analyzer(symbolMap[code[i].label]-1,i,nstages,EU_until,EUs_until,0,code,regMap);
                    cout<<old_div<<num<<": Salto non preso"<<endl;
                    code[symbolMap[code[i].label]-1].decode=tmp;    //Restore decode time of instruction after jump
                }
                else if(type=="CALL"){
                    regMap[r1].inst=num;
                    if(EU_until>=dec+1) EU_until++;
                    else EU_until=dec+1;
                    regMap[r1].until=EU_until;
                }
               // if( find(already.begin(), already.end(), i)!= already.end() ) break;
                if (i < code.size() - 1) code[i + 1].decode = dec + 1;  //Update decode of next instr.
            }
            else if (unt0==-1) busyReg=r1;
            else if(unt1==-1) busyReg=r0;
            else{  //Both can potentially cause dependency, choose worst case
                if(unt0>=unt1) busyReg=r0;
                else busyReg=r1;
            }
            if(busyReg>0) { // Runs only if there's at least one dependency
                int u=regMap[busyReg].until;
                int blockingInst=regMap[busyReg].inst;
                if (isConditional(type)) { //IF
                    if (u >= dec) { // EU-IU Dependency
                        printIUEU(blockingInst, num, u+1 -dec,busyReg);
                        if (i < code.size() - 1) code[i + 1].decode = u + 2;
                    } else if (i < code.size() - 1){
                        //if( find(already.begin(), already.end(), i)!= already.end() ) break;
                        code[i + 1].decode = dec + 1;
                    }

                    printJump(num);
                    int tmp = code[symbolMap[code[i].label]-1].decode;
                    code[symbolMap[code[i].label]-1].decode=code[i+1].decode;
                    string old_div=divider;
                    divider.append("\t");
                    analyzer(symbolMap[code[i].label]-1,i,nstages,EU_until,EUs_until,0,code,regMap);
                    cout<<old_div<<num<<": Salto non preso"<<endl;

                    code[symbolMap[code[i].label]-1].decode=tmp;
                }
                else if (isAritm(type) || type=="CALL") {   //MOVE,ADD_I,SUB_I,MUL_I,DIV_I,MOD_I
                    if ( u >= dec + 1 && isLong(blockingInst)) {
                        printEUEU(blockingInst, num, u - EU_until,busyReg);
                        regMap[r1].inst = num;  //Claim register
                        if(isLong(num)){
                            if(EU_until>=u+1) EU_until+=nstages;
                            else EU_until=u+1+nstages;
                            EUs_until=EUs_until+nstages+1;
                            regMap[r1].until = EUs_until;
                        }
                        else{
                            if (EU_until >= u + 1) EU_until++;
                            else EU_until = u + 1;
                            regMap[r1].until=EU_until;
                        }
                    } else if (u >= dec + 1 && !isLong(blockingInst)) {  //Wait for EU to be free, then proceed
                        regMap[r1].inst = num;  //Claim register
                        if (EU_until >= dec + 1) EU_until++;
                        else EU_until = dec + 1;
                        if (isLong(num)) {
                            EUs_until = EU_until + nstages;
                            regMap[r1].until = EUs_until;
                        } else
                            regMap[r1].until = EU_until;
                       // if( find(already.begin(), already.end(), i)!= already.end() ) break;
                    } else if ( u < dec + 1) {
                        regMap[r1].inst = num;
                        if (EU_until >= dec + 1) EU_until++;
                        else EU_until = dec + 1;
                        if (isLong(num)) {
                            EUs_until = EU_until + nstages;
                            regMap[r1].until = EUs_until;
                        } else
                            regMap[r1].until = EU_until;
                        //if( find(already.begin(), already.end(), i)!= already.end() ) break;
                    }
                    if (i < code.size() - 1) code[i + 1].decode = dec + 1;
                }
                else if(type=="STORE_I" && u>=dec){ //EU-IU Dependency
                    printIUEU(blockingInst, num, u + 1 - dec,busyReg);
                    if (i < code.size() - 1) code[i + 1].decode = u + 2;
                }
                else if(type=="LOAD_I"){
                    regMap[r1].inst = num; //Claim register
                    if(busyReg==r0){
                        if(u>=dec){ //EU-IU Dependency
                            printIUEU(blockingInst, num, u - dec+1,busyReg);
                            EU_until=u+3;
                            regMap[r1].until=EU_until;
                            if (i < code.size() - 1) code[i + 1].decode = u + 2;
                        }
                        else{
                            EU_until=dec+2;
                            regMap[r1].until=EU_until;
                            //if( find(already.begin(), already.end(), i)!= already.end() ) break;
                            if (i < code.size() - 1) code[i + 1].decode = dec + 1;
                        }
                    }
                    else{
                        if(u>=dec+2 && isLong(i)){ //EU-EU Dependency
                            printEUEU(blockingInst, num, u -(EUs_until-1),busyReg);
                            EU_until=u+1;
                            regMap[r1].until=EU_until;
                        }
                        else{
                            EU_until=dec+2;
                            regMap[r1].until=EU_until;
                            //if( find(already.begin(), already.end(), i)!= already.end() ) break;
                        }
                        if (i < code.size() - 1) code[i + 1].decode = dec + 1;

                    }
                }
            }
        }
        /*Three registers to check*/
        if(nregs==3){
            int r0=code[i].regs[0];
            int r1=code[i].regs[1];
            int r2=code[i].regs[2];
            int unt0=check(r0,regMap);
            int unt1=check(r1,regMap);
            int unt2=check(r2,regMap);
            int busyReg=-1;
            if(unt0==-1 && unt1==-1 && unt2==-1){   //No dependencies
                //if( find(already.begin(), already.end(), i)!= already.end() ) break;
                if(isAritm(type)){  //ADD,SUB,MUL,DIV,MOD
                    regMap[r2].inst=num;    //Claim register
                    if(isLong(num)){
                        if(EU_until>=dec+1) EU_until++;
                        else EU_until=dec+1;
                        EUs_until=EU_until+nstages;
                        regMap[r2].until=EUs_until;
                    }
                    else{
                        if(EU_until>=dec+1) EU_until++;
                        else EU_until=dec+1;
                        regMap[r2].until=EU_until;
                    }
                }
                else if(type=="LOAD"){
                    regMap[r2].inst=num;    //Claim register
                    if(EU_until>=dec+2) EU_until++;
                    else EU_until=dec+2;
                    regMap[r2].until=EU_until;
                }
                if (i < code.size() - 1) code[i + 1].decode = dec + 1;
            }
            else if(unt0!=-1 && unt1==-1 && unt2==-1) busyReg=r0;
            else if(unt0==-1 && unt1!=-1 && unt2==-1) busyReg=r1;
            else if(unt0==-1 && unt1==-1) busyReg=r2;
            else if(unt0!=-1 && unt1!=-1 && unt2==-1){
                if(unt0>=unt1) busyReg=r0;
                else busyReg=r1;
            }
            else if(unt0==-1 && unt1!=-1 && unt2!=-1){
                if(unt1>=unt2) busyReg=r1;
                else busyReg=r2;
            }
            else if(unt0!=-1 && unt1==-1 && unt2!=-1){
                if(unt0>=unt2) busyReg=r1;
                else busyReg=r2;
            }
            else {
                if (unt0 >= unt1 && unt0 >= unt2) busyReg = r0;
                else if (unt1 >= unt2) busyReg = r1;
                else busyReg = r2;
            }
            if(busyReg>0) { //Runs only if there's at least one dependency
                int u=regMap[busyReg].until;
                int blockingIns=regMap[busyReg].inst;
                if (isAritm(type)) {   //ADD,SUB,MUL,DIV,MOD
                    if ( u >= dec + 1 && isLong(blockingIns)) {  //EU-EU Dependency
                        printEUEU(blockingIns, num, u - EU_until,busyReg);
                        regMap[r2].inst = num;  //Claim register
                        if(isLong(num)){
                            if(EU_until>=dec+1) EU_until+=nstages;
                            else EU_until=dec+1+nstages;
                            EUs_until=EU_until+nstages+1;
                            regMap[r2].until = EUs_until;
                        }
                        else{
                            if (EU_until>= u + 1) EU_until++;
                            else EU_until = u + 1;
                            regMap[r2].until=EU_until;
                        }
                    }
                    else if (u >= dec + 1 && !isLong(blockingIns)) {//Wait for EU to be free, then proceed
                        regMap[r2].inst = num;  //Claim register
                        if (EU_until>= dec + 1) EU_until++;
                        else EU_until = dec + 1;
                        if (isLong(num)) {
                            EUs_until = EU_until + nstages;
                            regMap[r2].until = EUs_until;
                        } else
                            regMap[r2].until = EU_until;
                        //if( find(already.begin(), already.end(), i)!= already.end() ) break;
                    } else if ( u < dec + 1) {
                        regMap[r2].inst = num;
                        if (EU_until >= dec + 1) EU_until++;
                        else EU_until = dec + 1;
                        if (isLong(num)) {
                            EUs_until = EUs_until + nstages;
                            regMap[r2].until = EUs_until;
                        } else
                            regMap[r2].until = EU_until;
                        //if( find(already.begin(), already.end(), i)!= already.end() ) break;
                    }
                    if (i < code.size() - 1) code[i + 1].decode = dec + 1;
                }
                else if(type=="STORE" && u>=dec){ //EU-IU Dependency
                    printIUEU(blockingIns, num, u + 1 - dec,busyReg);
                    if (i < code.size() - 1) code[i + 1].decode = u + 2;
                }
                else if(type=="LOAD"){
                    regMap[r2].inst = num;  //Claim register
                    if(busyReg==r0 || busyReg==r1){
                        if(u>=dec){ //EU-IU Dependency
                            printIUEU(blockingIns, num, u - dec+1,busyReg);
                            EU_until=u+3;
                            regMap[r2].until=EU_until;
                            if (i < code.size() - 1) code[i + 1].decode = u + 2;
                        }
                        else{
                            EU_until=dec+2;
                            regMap[r2].until=EU_until;
                            //if( find(already.begin(), already.end(), i)!= already.end() ) break;
                            if (i < code.size() - 1) code[i + 1].decode = dec + 1;
                        }
                    }
                    else{
                        if(u>=dec+2 && isLong(i)){ //EU-EU Dependency
                            printEUEU(blockingIns, num, u -(EUs_until-1),busyReg);
                            EU_until=u+1;
                            regMap[r2].until=EU_until;
                            if (i < code.size() - 1) code[i + 1].decode = dec + 1;
                        }
                        else{
                            EU_until=dec+2;
                            regMap[r2].until=EU_until;
                            //if( find(already.begin(), already.end(), i)!= already.end() ) break;
                            if (i < code.size() - 1) code[i + 1].decode = dec + 1;
                        }
                    }
                }
            }
        }   //Close nregs check
        already.push_back(i);
    }   //Close for
    return;
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
            /*Memorize association*/
            symRegTable[stoi(v[2].substr(1,v[2].size()-1))]=v[1];
            substitute("tmp.drisc", v[1],v[2]);
        }
    }

    yyin=fopen("tmp.drisc","r");
    cout << "Start parsing..." << endl;
    yyparse();
    cout << endl;
    remove("tmp.drisc");
    cout << "Start analyzing..." << endl;
    analyzer(0,-1,nstages,0,0,0,prog,rMap);
    return 0;
}

