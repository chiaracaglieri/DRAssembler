#ifndef _STATE_H
#define _STATE_H

#endif

#include <fstream>
#include <map>
#include <unordered_map>
#include <bitset>

using namespace std;

extern map<int,int> regTable;
extern map<int,int> memTable;
extern map<int,string> memCode;
extern int start;
extern int loc;



void loadRegisters(string filename );
void loadMemory(string filename );
void loadProgram(string filename);

extern unordered_map<string,string> opTable;
void loadOptable();