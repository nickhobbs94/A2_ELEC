/* Handy conversions */

#ifndef CONVERSIONS_H
#define CONVERSIONS_H

/* Function prototypes */
int intfromstring(char string[]);

/* Functions */
int intfromstring(char string[]){
    int result=0;
    
    int i;
    for (i=0; string[i]!='\0'; i++){
        if (string[i]<'0' || string[i] > '9'){
            return -1;
        }
        
        result += string[i] % 0x10;
    }
    return result;
}

#endif
