#ifdef __cplusplus
extern "C" {
#endif
#include <stdio.h>
#include "symbols.h"
#ifdef __cplusplus
} // closing brace for extern "C"

#endif

void updateLC(int value){
    lc=value;
    printf("Updated lc value to %d\n", lc);
}

void checkSymbol(char* label, int lc1){

}