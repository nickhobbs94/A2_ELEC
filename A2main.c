#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include "wordsplit.h"
#include "alt_types.h"

#define PUTTY_LINE_LENGTH 50


alt_32 puttyGetline(alt_8 string[], alt_32 lineLength);

int main() {
	alt_32 i=0;
	alt_8 string[PUTTY_LINE_LENGTH];
	
	alt_32 stringlength = puttyGetline(string,PUTTY_LINE_LENGTH);
	alt_8** array_of_words;
	array_of_words = malloc(STRING_PARSER_MAXNUM_WORDS(stringlength));
	alt_8 numwords = string_parser(string,array_of_words);
	
	for (i=0;i<numwords;i++){
		printf("%s\n",*(array_of_words+i));
	}
	return 0;
}

alt_32 puttyGetline(alt_8 string[], alt_32 lineLength){
    alt_32 i=0;
    alt_32 uart_pointer = open("/dev/uart_0", O_RDWR, O_NONBLOCK);
    
    do {
        read(uart_pointer,&string[i],1);
        write(uart_pointer,&string[i],1);
        i++;
    } while(string[i-1]!='\r' && i<lineLength); // loop until newline or line length is reached
    
    string[i-1]='\0'; // null terminate the string
    close(uart_pointer);
    return i;
}

