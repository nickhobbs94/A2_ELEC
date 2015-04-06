#ifndef TERMINALFUNCTIONS_H
#define TERMINALFUNCTIONS_H
#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include "conversions.h"
#define NUMBER_OF_LEDS 18
#define BINARY_BITS_IN_DECIMAL(decimalNumber) log2(decimalNumber)+1

/* Function prototypes */
int echo(int argc, char* argv[]);
int add(int argc, char* argv[]);
int ledr(int argc, char* argv[]);

/* Functions */
int echo(int argc, char* argv[]){
    int i;
    for (i=1; i<argc; i++){
        printf("%s ",argv[i]);
    }
    printf("\n");
    return 0;
}

int add(int argc, char* argv[]){
    int sum=0;
    int i;
    int temp;
    for (i=1; i<argc; i++){
        temp = intfromstring(argv[i]);
        if (temp == -1){
            return -1;
        }
        sum += temp;
        
    }
    
    
    printf("%d\n",sum);
    return 0;
}

int ledr(int argc, char* argv[]){
    int i, j;
    int dec;
    for (i=1; i<argc; i++){
        dec = intfromstring(argv[i]);
        if (dec == -1){
            return -1;
        }     
    }
    int* binary = calloc(BINARY_BITS_IN_DECIMAL(dec),sizeof(bool));
    decimaltobinary(binary, dec);
    //Enter stuff in here to display on red LEDs on dev board
    for(j=BINARY_BITS_IN_DECIMAL(dec)-1; j>=0; --j){
        printf(binary[j]?"1":"0");
    }
    printf("\n");
    free(binary);
    return 0;
}

#endif