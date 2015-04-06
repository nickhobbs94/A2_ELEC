/* Handy conversions */

#ifndef CONVERSIONS_H
#define CONVERSIONS_H
#include <math.h>
#include <string.h>
#define BINARY_DIVISOR 2

/* Function prototypes */
int intfromstring(char string[]);
void decimaltobinary(int*binaryArray, int decimal);

/* Functions */
int intfromstring(char string[]){
    int result=0;
    
    int i;
    int place=0;
    for (i=strlen(string)-1; i>=0; i--){
        
        if (string[i]<'0' || string[i] > '9'){
            return -1;
        }
        
        result += string[i] % 0x10 * pow(10,place);
        place++;
    }
    return result;
}

void decimaltobinary(int* binaryArray, int decimal){
    int i;
    for(i=0; decimal>0; i++){
        binaryArray[i] = (decimal % BINARY_DIVISOR);
        decimal /= BINARY_DIVISOR;
    }
}

#endif