/**
 * @file int_util.h
 * @author Chiara Caglieri
 * @brief Header file containing the declaration of utility functions for the interpreter
 */

#include <fstream>
#include <map>
#include <unordered_map>
#include <bitset>

using namespace std;

extern unordered_map<string,string> opTable;    /**<The table containing the opcodes*/
extern map<int,int> regTable;                   /**<The register table*/
extern map<int,int> memTable;                   /**<The memory table*/
extern map<int,string> memCode;                 /**<The table containing the instructions*/
extern int start;                               /**<The value associated with the pseudo-inst. START*/
extern int pc;                                  /**<The program counter*/

void loadRegisters(string filename );
void loadMemory(string filename );
void loadProgram(string filename);
void loadOptable();

string checkSym(int reg);