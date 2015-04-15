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
alt_32 switch_function(alt_32 argc, alt_8* argv[]);
alt_32 ls_path(alt_32 argc, alt_8* argv[]);
alt_32 change_dir(alt_32 argc, alt_8* argv[]);
alt_32 tf_mount(alt_32 argc, alt_8* argv[]);
alt_32 tf_unmount(alt_32 argc, alt_8* argv[]);

alt_32 make_directory(alt_32 argc, char* argv[]);
alt_32 delete_file(alt_32 argc, char* argv[]);
alt_32 write_new_file(alt_32 argc, char* argv[]);

alt_32  read_file(alt_32 argc, char* argv[]);
alt_32  copy_file(alt_32 argc, char* argv[]);

/* ----------------------------------- Functions ----------------------------------- */

/* Print to the LCD */
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

/* Add two or more numbers and show the sum on the LCD */
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

/* Updates the red LEDs to be the binary representation of a number */
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

/* shows the switch configuration as hex on the 7-seg display */
alt_32 switch_function(alt_32 argc, alt_8* argv[]){
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

/* lists the contents of a directory */
alt_32 ls_path(alt_32 argc, alt_8* argv[]){
	EmbeddedFileSystem* efsl;
	DirList list;
	
	efsl = *(SD_mount());

	if (efsl==NULL) return -1;

	/* Get absolute path */
	char* path;
	path = SD_getCurrentPath();

	if (argc>1){
		SD_updatePath(path,argv[1]);
	}
	printf("PATH: %s\n",path);

	/* Read directory */
	alt_8 checkpath = ls_openDir(&list,&(efsl->myFs),path);
	if (checkpath!=0){
		printf("Unable to read path: %s\n",path);
		return -1;
	}

	char attribute;
	while(ls_getNext(&list)==0){
		attribute = SD_getFileAttribute(list.currentEntry.Attribute);
		printf("%s \t%c \t(%li)\n",
				list.currentEntry.FileName,
				attribute,
				list.currentEntry.FileSize
				);
	}
	//SD_unmount();
	return 0;
}

/* changes the current directory */
alt_32 change_dir(alt_32 argc, alt_8* argv[]){
	EmbeddedFileSystem* efsl;
	DirList list;
	efsl = *(SD_mount());


	char* path;
	path = SD_getCurrentPath();

	/* check it is a valid directory that the user wants to change the path to */
	alt_8 checkpath=0;
	if (argc>1){
		checkpath = ls_openDir(&list,&(efsl->myFs),argv[1]);
		if (checkpath==0){
			SD_updatePath(path,argv[1]);
		} else {
			printf("Invalid path: %s\n",argv[1]);
		}
	}

	printf("PATH: %s\n",path);
	return 0;
}

alt_32 tf_mount(alt_32 argc, alt_8* argv[]){
	SD_mount();
	return 0;
}

alt_32 tf_unmount(alt_32 argc, alt_8* argv[]){
	SD_unmount();
	return 0;
}

/* -------------- begin merge ---------------------- */

alt_32 make_directory(alt_32 argc, char* argv[]){
	EmbeddedFileSystem* efsl;
	DirList list;

	efsl = *(SD_mount());

	if (efsl==NULL)
		return -1;

	/* Get absolute path */
	char path[SD_MAX_PATH_LENGTH];
	memset(path,'\0',sizeof(path));
	strcpy(path,argv[1]);
	if (argc==3){
		strcat(path,"/");
		strcat(path,argv[2]);
	}
	if(makedir(&(efsl->myFs), path)==0){
		makedir(&(efsl->myFs), path);
	} else {
		printf("This directory already exists\n");
	}

	//SD_unmount();

	return 0;
}

alt_32 delete_file(alt_32 argc, char* argv[]){
	EmbeddedFileSystem* efsl;
	DirList list;

	efsl = *(SD_mount());

	if (efsl==NULL)
		return -1;

	/* Get absolute path */
	char path[SD_MAX_PATH_LENGTH];
	memset(path,'\0',sizeof(path));
	strcpy(path,argv[1]);
	if (argc==3){
		strcat(path,"/");
		strcat(path,argv[2]);
	}
	rmfile(&(efsl->myFs), path);

	//SD_unmount();

	return 0;
}

alt_32 write_new_file(alt_32 argc, char* argv[]){
	EmbeddedFileSystem* efsl;
	DirList list;
	File file;
	euint8 *write_buffer="Text text text words";

	efsl = *(SD_mount());

	if (efsl==NULL)
		return -1;

	/* Get absolute path */
	char path[SD_MAX_PATH_LENGTH];
	memset(path,'\0',sizeof(path));
	strcpy(path,argv[1]);
	if (argc==3){
		strcat(path,"/");
		strcat(path,argv[2]);
	}
	if(file_fopen(&file, &(efsl->myFs), path, 'w')!=0){
				printf("Could not open file for writing\n");
				return 0;
	}
	printf("File opened for writing.\n");

	if(file_write(&file,strlen(write_buffer),write_buffer)==strlen(write_buffer)){
		printf("File written.\n");
	} else {
		printf("could not write file.\n");
	}

	file_fclose(&file);
	//SD_unmount();

	return 0;
}

alt_32  read_file(alt_32 argc, char* argv[]){
	EmbeddedFileSystem* efsl;
	DirList list;
	File file;
	euint8 buffer[512];
	euint16 e,f;

	efsl = *(SD_mount());

	if (efsl==NULL)
		return -1;

	/* Get absolute path */
	char path[SD_MAX_PATH_LENGTH];
	memset(path,'\0',sizeof(path));
	strcpy(path,argv[1]);
	if (argc==3){
		//strcat(path,"/");
		strcat(path,argv[2]);
	}
	if(file_fopen(&file, &(efsl->myFs), path, 'r')!=0){
				printf("Could not open file for reading\n");
				return -1;
	}
	printf("File opened for reading.\n");

	while((e=file_read(&file,512,buffer))){
		for(f=0;f<e;f++){
			printf("%c",buffer[f]);
		}
	}

	file_fclose(&file);
	//SD_unmount();

	return 0;
}

alt_32 copy_file(alt_32 argc, char* argv[]){
	EmbeddedFileSystem* efsl;
	DirList list;
	File file1, file2;
	euint8 buffer[512];
	euint16 e,f;

	efsl = *(SD_mount());

	if (efsl==NULL)
		return -1;

	/* Get absolute path of read file */
	char read_file_path[SD_MAX_PATH_LENGTH];
	memset(read_file_path,'\0',sizeof(read_file_path));
	strcpy(read_file_path,argv[1]);
	//strcat(read_file_path,"/");
	strcat(read_file_path,argv[3]);
	/* Get absolute path of write file */
	char write_file_path[SD_MAX_PATH_LENGTH];
	memset(write_file_path,'\0',sizeof(read_file_path));
	strcpy(write_file_path,argv[2]);
	//strcat(write_file_path,"/");
	strcat(write_file_path,argv[3]);

	if(file_fopen(&file1, &(efsl->myFs), read_file_path, 'r')!=0){
		printf("Could not open file for reading\n");
		return -1;
	}
	printf("File opened for reading.\n");

	//while(())){

	//}
	e=file_read(&file1,512,buffer);
	if(file_fopen(&file2, &(efsl->myFs), write_file_path, 'w')!=0){
		printf("Could not open file for writing\n");
		return -1;
	}
	printf("File opened for writing.\n");

	if(file_write(&file2,e,buffer)==e){
		printf("File written.\n");
	} else {
		printf("could not write file.\n");
	}

	file_fclose(&file1);
	file_fclose(&file2);
	//SD_unmount();

	return 0;
}


#endif

