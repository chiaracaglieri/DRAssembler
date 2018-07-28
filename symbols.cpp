#include <stdio.h>
#include <unordered_map>

using namespace std;

std::unordered_map<std::string, int> symTable;

extern "C" void checkSymbol(char* lbl, int lc_1){
    if (symTable.find(lbl)!=symTable.end())
        printf("Label already in symtab...\n");
    else{
        symTable[lbl]=lc_1;
        printf("Inserted label %s with lc %d\n", lbl, symTable[lbl]);
    }
}