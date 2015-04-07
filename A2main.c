#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include "wordsplit.h"

#define PUTTY_LINE_LENGTH 50

//typedef char alt_8;
//typedef int alt_32;

int puttyGetline(char string[], int lineLength);

int main(int argc, char* argv[]) {
	int i=0;
	char string[PUTTY_LINE_LENGTH];
	
	int stringlength = puttyGetline(string,PUTTY_LINE_LENGTH);
	char** array_of_words;
	array_of_words = malloc(STRING_PARSER_MAXNUM_WORDS(stringlength));
	char numwords = string_parser(string,array_of_words);
	
	for (i=0;i<numwords;i++){
		printf("%s\n",*(array_of_words+i));
	}
	return 0;
}

int puttyGetline(char string[], int lineLength){
    int i=0;
    int uart_pointer = open("/dev/uart_0", O_RDWR, O_NONBLOCK);
    
    do {
        read(uart_pointer,&string[i],1);
        write(uart_pointer,&string[i],1);
        i++;
    } while(string[i-1]!='\r' && i<lineLength); // loop until newline or line length is reached
    
    string[i-1]='\0'; // null terminate the string
    close(uart_pointer);
    return i;
}

