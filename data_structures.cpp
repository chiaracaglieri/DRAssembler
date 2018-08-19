//
// Created by chiar on 24/07/2018.
//

#include <iostream>
#include <vector>
#include "util.h"
#include <unordered_map>

/*Optable*/
std::vector<struct opcode> optable{{"memloc",0},{"memlocs",1},{"regval",2},{"loc",3},{"start",4},{"end",5},{"clear",6},{"load",7},{"store",8},
                         {"add",9}, {"sub",10},{"mul",11},{"incr",12},{"if=",13},{"if>",14},{"if<",15}};

std::unordered_map<std::string, int> symTable;