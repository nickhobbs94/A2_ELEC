#include <stdio.h>
#include "terminalParse.h"

int main(int argc, char* argv[]){
    char string[] = " echo world other words :)";
    char** array_of_words;
    array_of_words = malloc(STRING_PARSER_MAXNUM_WORDS(string));
    //printf("Strlen: %lu ------- Pointer: %lu\n",STRING_PARSER_MAXNUM_WORDS(string),(unsigned long) array_of_words);
    if (array_of_words==NULL){
        printf("Oh shit\n");
        return 0;
    }
    unsigned char num_words = string_parser(string,array_of_words);
    /*for (int i=0; i<num_words; i++){
        printf("%s\n", array_of_words[i]);
    }*/
    command_interpreter(num_words, array_of_words);
    free(array_of_words);
    return 0;
}



