/*
	Every function that is implemented on our terminal will be in this file.
	This allows for easy editing and extensions.
*/

#ifndef TERMINALFUNCTIONS_H
#define TERMINALFUNCTIONS_H

/* Includes */
#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include "conversions.h"
#include "io.h"
#include "system.h"
#include "alt_types.h"
#include "LCD_Control.h"
#include "efs.h"
#include "ls.h"
#include "SD_functions.h"

/* Magic numbers */
#define NUMBER_OF_LEDS 18
#define BINARY_BITS_IN_DECIMAL(decimalNumber) log2(decimalNumber)+1

/* Function prototypes */
alt_32 echo(alt_32 argc, alt_8* argv[]);
alt_32 add(alt_32 argc, alt_8* argv[]);
alt_32 ledr(alt_32 argc, alt_8* argv[]);
alt_32 switch_function(alt_32 argc, char* argv[]);
alt_32 lsroot(alt_32 argc, char* argv[]);

/* ----------------------------------- Functions ----------------------------------- */

alt_32 echo(alt_32 argc, alt_8* argv[]){
	alt_32 i;
	LCD_Init();
	for (i=1; i<argc; i++){
		//printf("%s ",argv[i]);
		LCD_Show_Text((char*)argv[i]);
		LCD_Show_Text(" ");
	}
	//printf("\n");
	return 0;
}

alt_32 add(alt_32 argc, alt_8* argv[]){
	alt_32 sum=0;
	alt_32 i;
	alt_32 temp;
	LCD_Init();
	for (i=1; i<argc; i++){
		LCD_Show_Text((char*)argv[i]);
		if (i<argc-1) LCD_Show_Text("+");

		temp = intfromstring(argv[i]);
		if (temp == -1){
			return -1;
		}
		sum += temp;
		
	}
	//printf("%d\n",(int)sum);
	LCD_Line2();
	LCD_Show_Text("=");
	LCD_Show_Decimal(sum);
	return 0;
}

alt_32 ledr(alt_32 argc, alt_8* argv[]){
	alt_32 i;
	alt_32 dec=0;
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
	alt_32* hex=calloc(8,sizeof(alt_32));
	decimaltohex(hex,dec);
	static alt_u8  Map[] = {
		63, 6, 91, 79, 102, 109, 125, 7,
		127, 111, 119, 124, 57, 94, 121, 113
	};  // 0,1,2,3,4,5,6,7,8,9,a,b,c,d,e,f
	for(i=0;i<8;++i) {
		//printf("%x\n",hex[i]);
		IOWR(SEG7_DISPLAY_BASE,i,Map[hex[i]]);
	}
	free(hex);
	return 0;
}

alt_32 lsroot(alt_32 argc, char* argv[]){
	EmbeddedFileSystem* efsl;
	DirList list;
	
	efsl = SD_mount();

	if (efsl==NULL) return -1;

	/* Get absolute path */
	char* path;
	path = SD_getCurrentPath();

	if (argc>1){
		SD_updatePath(path,argv[1]);
	}
	printf("PATH: %s\n",path);

	/* Read directory */
	ls_openDir(&list,&(efsl->myFs),path);
	char attribute;
	while(ls_getNext(&list)==0){
		attribute = SD_getFileAttribute(list.currentEntry.Attribute);
		printf("%s \t%c \t(%li)\n",
				list.currentEntry.FileName,
				attribute,
				list.currentEntry.FileSize
				);
	}
	SD_unmount();
	return 0;
}

#endif

