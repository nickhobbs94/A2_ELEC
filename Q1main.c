#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include "wordsplit.h"

int main(int argc, char* argv[]) {
	/* initialise variables */
	int uart_rdwr = open("/dev/uart_0", O_RDWR,O_NONBLOCK);
	int i=0;
	char string[50];
	/* Loop will continue displaying keyboard presses in putty and NIOS until the ESC key is pressed */
	do {
		read(uart_rdwr,&string[i],1);
		/* Robustness:  If ESC key pressed then breaks infinite loop, if ENTER pressed, will act like real enter by conducting a new line and carriage return */
		if(string[i]=='\r'){
			write(uart_rdwr,&string[i],1);
			string[i]='\n';
			write(uart_rdwr,&string[i],1);
			string[i]='\0';
		}
		/* Functionality: writes character to uart */
		write(uart_rdwr,&string[i],1);

		i++;
	} while(string[i-1]!='\0');
	//printf("\nString: %s\n",string);
	char** array_of_words;
	array_of_words = malloc(STRING_PARSER_MAXNUM_WORDS(string));
	unsigned char numwords = string_parser(string,array_of_words);
	for (i=0;i<numwords;i++){
		printf("%s\n",*(array_of_words+i));
	}
	return 0;
}

