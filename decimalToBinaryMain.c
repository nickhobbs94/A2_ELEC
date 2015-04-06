#include <stdio.h>
#include "terminalParse.h"

int main(int argc, char* argv[]){
    char string[] = "ledr 10";
    char** array_of_words;
    array_of_words = malloc(STRING_PARSER_MAXNUM_WORDS(string));
    if (array_of_words==NULL){
        printf("Oh shit\n");
        return 0;
    }
    unsigned char num_words = string_parser(string,array_of_words);
    command_interpreter(num_words, array_of_words);
    free(array_of_words);
    return 0;
}

