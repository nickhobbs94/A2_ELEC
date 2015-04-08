/* Handy conversions */

#ifndef CONVERSIONS_H
#define CONVERSIONS_H
#include <math.h>
#include <string.h>
#include "alt_types.h"
#define BINARY_DIVISOR 2
#define HEX_DIVISOR 16

/* Function prototypes */
alt_32 intfromstring(alt_8 string[]);
void decimaltobinary(alt_32* binaryArray, alt_32 decimal);
void decimaltohex(alt_32* hexArray, alt_32 decimal);

/* Functions */
alt_32 intfromstring(alt_8 string[]){
    alt_32 result=0;
    
    alt_32 i;
    alt_32 place=0;
    for (i=strlen(string)-1; i>=0; i--){
        
        if (string[i]<'0' || string[i] > '9'){
            return -1;
        }
        
        result += string[i] % 0x10 * pow(10,place);
        place++;
    }
    return result;
}

void decimaltobinary(alt_32* binaryArray, alt_32 decimal){
    alt_32 i;
    for(i=0; decimal>0; i++){
        binaryArray[i] = (decimal % BINARY_DIVISOR);
        decimal /= BINARY_DIVISOR;
    }
}

void decimaltohex(alt_32* hexArray, alt_32 decimal){
    alt_32 i;
    for(i=0; decimal>0; i++){
        hexArray[i] = (decimal % HEX_DIVISOR);
        decimal /= HEX_DIVISOR;
    }
}

#endif
