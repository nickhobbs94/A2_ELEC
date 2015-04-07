/*
terminalParse.h
*/
#ifndef TERMINALPARSE_H
#define TERMINALPARSE_H
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "terminalFunctions.h"
#include "alt_types.h"
#define STRING_PARSER_MAXNUM_WORDS(string) string/2+1

/* Function prototypes */
alt_8 string_parser(alt_8* string, alt_8* array_of_words[]);
alt_32 command_interpreter(alt_32 argc, alt_8* argv[]);

/* Functions */
alt_32 command_interpreter(alt_32 argc, alt_8* argv[]){
    struct terminal_functions {
        alt_u8* command_string;
        alt_32 (*command_function)(alt_32 argc, alt_8* argv[]);
    } terminal_commands[] = {
        {"echo",echo}, {"add",add}, {"ledr",ledr}, {NULL,NULL}
    };
    
    alt_32 i;
    for (i=0; terminal_commands[i].command_string != NULL; i++){
        if (strcmp(terminal_commands[i].command_string, argv[0]) == 0){
            terminal_commands[i].command_function(argc, argv);
        }
    }
    return 0;
}


/*
USAGE: alt_8 string_parser(alt_8* string, alt_8* array_of_words[]);
GOTCHAS: the size of array_of_words should be malloc'd to be able to hold all the words, use at your own risk
SUGGESTION: use STRING_PARSER_MAXNUM_WORDS(string) to return the suggested size of array_of_words
*/
alt_8 string_parser(alt_8* string, alt_8* array_of_words[]){
    if (*(string)=='\0'){
        //printf("ERROR: Empty string\n");
        return 0;   // return 0 if empty string
    }
    alt_8 count = 0;
    alt_32 string_length = strlen(string);
    
    if (*(string)!=' '){    // if the first character is not a space then start the first word there
        count++;
        array_of_words[0] = string;
    }
    alt_32 i;
    for (i=0; i<string_length; i++){    // incrememnt through each character of the string
        if (*(string+i)==' '){    // if the current character is a space
            *(string+i)='\0';     // then replace the space with a null terminator
            // and if the following character is not the null terminator or another space
            if(*(string+i+1)!='\0' && *(string+i+1)!=' '){
                array_of_words[count]=string+i+1;  // then set the beginning of the next word to the next space
                count++;
            }
        } else if (*(string+i)=='\0'){  // or if the current character is the null terminator then end the for loop
            i=string_length;
        }
    }
    return count;  // return the number of words found
}

#endif

