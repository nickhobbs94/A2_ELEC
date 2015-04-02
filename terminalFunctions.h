#ifndef TERMINALFUNCTIONS_H
#define TERMINALFUNCTIONS_H
#include <stdio.h>

/* Function prototypes */
int echo(int argc, char* argv[]);

/* Functions */
int echo(int argc, char* argv[]){
    int i;
    for (i=0; i<argc; i++){
        printf("%s ",argv[i]);
    }
    printf("\n");
    return 0;
}

#endif
