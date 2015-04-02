#ifndef TERMINALFUNCTIONS_H
#define TERMINALFUNCTIONS_H
#include <stdio.h>
#include "conversions.h"

/* Function prototypes */
int echo(int argc, char* argv[]);
int add(int argc, char* argv[]);

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
    for (int i=1; i<argc; i++){
        temp = intfromstring(argv[i]);
        if (temp == -1){
            return -1;
        }
        sum += temp;
    }
    
    
    printf("%d\n",sum);
    return 0;
}

#endif
