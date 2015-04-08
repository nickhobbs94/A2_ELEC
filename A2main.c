#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include "terminalParse.h"
#include "alt_types.h"

#define PUTTY_LINE_LENGTH 50


alt_32 puttyGetline(alt_8 string[], alt_32 lineLength);


int main() {
	while(1){
		alt_8 string[PUTTY_LINE_LENGTH];
		alt_32 stringlength = puttyGetline(string,PUTTY_LINE_LENGTH);
		alt_8** array_of_words;
		array_of_words = malloc(STRING_PARSER_MAXNUM_WORDS(stringlength));
		alt_8 numwords = string_parser(string,array_of_words);
		command_interpreter(numwords, array_of_words);
		free(array_of_words);
	}
	return 0;
}

alt_32 puttyGetline(alt_8 string[], alt_32 lineLength){
    alt_32 i=0;
    alt_32 uart_pointer = open("/dev/uart_0", O_RDWR, O_NONBLOCK);
    
    do {
        read(uart_pointer,&string[i],1);
        if (string[i]=='\r'){
        	string[i]='\n';
        	write(uart_pointer,&string[i],1);
        	string[i]='\r';
        	write(uart_pointer,&string[i],1);
        } else if (string[i]==0x7f){
        	//string[i]='\0';
        	write(uart_pointer,&string[i],1);
            i=i-2;
        } else {
            write(uart_pointer,&string[i],1);
        }
        i++;
    } while(string[i-1]!='\r' && i<lineLength); // loop until newline or line length is reached
    
    string[i-1]='\0'; // null terminate the string
    printf("%s\n",string);
    close(uart_pointer);
    return i;
}

