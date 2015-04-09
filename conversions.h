/* 
	Handy conversions will live in this header file e.g. extracting an int from a string.
*/

#ifndef CONVERSIONS_H
#define CONVERSIONS_H

/* Includes */
#include <math.h>
#include <string.h>
#include "alt_types.h"

/* Magic numbers */
#define BINARY_DIVISOR 2
#define HEX_DIVISOR 16

/* Function prototypes */
alt_32 intfromstring(alt_8 string[]);
void decimaltobinary(alt_32* binaryArray, alt_32 decimal);
void decimaltohex(alt_32* hexArray, alt_32 decimal);
alt_8 charfromint(alt_32 number);

/* ----------------------------------- Functions ----------------------------------- */

alt_32 intfromstring(alt_8 string[]) {
	alt_32 result=0;
	alt_8 negative_flag = 0;
	
	/* If the string starts with a + or - then increment the pointer of the string so as to ignore it */
	if (string[0]=='+'){
		string+=1;
	} else if (string[0]=='-'){
		negative_flag = 1;
		string+=1;
	}
	
	alt_32 stringlength = strlen((char*)string);
	alt_32 i;
	
	
	for (i = 0; i<stringlength; i++){
		if (string[i] < '0' || string[i] > '9') {
			return 0; // return 0 if a non number is read from the string (check the user didnt just type 0)
		}
		result = result * 10;
		result = result + (string[i] & 0x0f);
	}
	
	/* Interpret sign */
	if (negative_flag == 1){
		result = result * -1;
	}
	
	return result;
}

alt_8 charfromint(alt_32 number){
//	alt_8 answer;
	if (number < 0 || number > 9){
		return -1;
	}

	alt_8 lookup_table[]={'0','1','2','3','4','5','6','7','8','9'};
	return lookup_table[number];
}

void decimaltobinary(alt_32* binaryArray, alt_32 decimal){
	alt_32 i;
	for(i=0; decimal>0; i++){
		binaryArray[i] = (decimal % BINARY_DIVISOR);
		decimal /= BINARY_DIVISOR;
	}
}

void decimaltohex(alt_32* hexArray, alt_32 decimal){
	alt_32 i;
	for(i=0; decimal>0; i++){
		hexArray[i] = (decimal % HEX_DIVISOR);
		decimal /= HEX_DIVISOR;
	}
}

#endif

