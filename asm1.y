%{
void yyerror (char*s);  //Called when there is za synctactical error
#include <stdio.h>
#include <stdlib.h>
#include "util.h"
#include "symbols.h"
%}

%union {
    int val;
    char* string;
    }
%token start end
%token <val> value
%token <string> register
%token <string> operation
%token <string> label

%%

