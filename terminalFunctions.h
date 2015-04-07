#ifndef TERMINALFUNCTIONS_H
#define TERMINALFUNCTIONS_H
#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include "conversions.h"
#include "io.h"
#include "system.h"
#include "alt_types.h"
#define NUMBER_OF_LEDS 18
#define BINARY_BITS_IN_DECIMAL(decimalNumber) log2(decimalNumber)+1

/* Function prototypes */
alt_32 echo(alt_32 argc, alt_8* argv[]);
alt_32 add(alt_32 argc, alt_8* argv[]);
alt_32 ledr(alt_32 argc, alt_8* argv[]);

/* Functions */
alt_32 echo(alt_32 argc, alt_8* argv[]){
    alt_32 i;
    for (i=1; i<argc; i++){
        printf("%s ",argv[i]);
    }
    printf("\n");
    return 0;
}

alt_32 add(alt_32 argc, alt_8* argv[]){
    alt_32 sum=0;
    alt_32 i;
    alt_32 temp;
    for (i=1; i<argc; i++){
        temp = intfromstring(argv[i]);
        if (temp == -1){
            return -1;
        }
        sum += temp;
        
    }
    printf("%d\n",(int)sum);
    return 0;
}

alt_32 ledr(alt_32 argc, alt_8* argv[]){
    alt_32 i;
    alt_32 dec;
    for (i=1; i<argc; i++){
        dec = intfromstring(argv[i]);
        if (dec == -1){
            return -1;
        }     
    }
   IOWR(LED_RED_BASE, 0, dec);
   return 0;
}

alt_32 switch_function(alt_32 argc, char* argv[]){
	alt_32 dec = IORD(SWITCH_PIO_BASE,0);
	alt_32 i;
	alt_32* hex=calloc(HEX_DIGIT_IN_DECIMAL(dec),sizeof(alt_32));
	decimaltohex(hex,dec);
	static alt_u8  Map[] = {
	        63, 6, 91, 79, 102, 109, 125, 7,
	        127, 111, 119, 124, 57, 94, 121, 113
	   };  // 0,1,2,3,4,5,6,7,8,9,a,b,c,d,e,f
	 for(i=0;i<HEX_DIGIT_IN_DECIMAL(dec);++i) {
		 printf("%x\n",hex[i]);
			IOWR(SEG7_DISPLAY_BASE,i,Map[hex[i]]);
	}
}

#endif
