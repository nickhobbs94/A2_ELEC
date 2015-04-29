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
#define MAX_LED_REGISTER 0x3ffff

/* Function prototypes */
alt_32 echo(alt_32 argc, alt_8* argv[]);
alt_32 add(alt_32 argc, alt_8* argv[]);
alt_32 ledr(alt_32 argc, alt_8* argv[]);
alt_32 switch_function(alt_32 argc, alt_8* argv[]);
alt_32 ls_path(alt_32 argc, alt_8* argv[]);
alt_32 change_dir(alt_32 argc, alt_8* argv[]);
alt_32 tf_mount(alt_32 argc, alt_8* argv[]);
alt_32 tf_unmount(alt_32 argc, alt_8* argv[]);
alt_32 make_directory(alt_32 argc, alt_8* argv[]);
alt_32 delete_file(alt_32 argc, alt_8* argv[]);
alt_32 write_new_file(alt_32 argc, alt_8* argv[]);
alt_32 read_file(alt_32 argc, alt_8* argv[]);
alt_32 copy_file(alt_32 argc, alt_8* argv[]);

/* ----------------------------------- Functions ----------------------------------- */

/* Print to the LCD and the console */
alt_32 echo(alt_32 argc, alt_8* argv[]){
	alt_32 i;
	LCD_Init();

	/* Loop over each argument provided to the function and print each one to the LCD and console */
	for (i=1; i<argc; i++){
		printf("%s \n",argv[i]);
		LCD_Show_Text(argv[i]);
		LCD_Show_Text((alt_8*) " ");
	}
	return 0;
}

/* Add two or more numbers and show the sum on the LCD */
alt_32 add(alt_32 argc, alt_8* argv[]){
	alt_32 sum = 0;
	alt_32 i;
	alt_32 temp;
	LCD_Init();

	for (i=1; i<argc; i++){
		/* Print each argument */
		LCD_Show_Text(argv[i]);
		/* Print a trailing '+' for all but the last argument */
		if (i<argc-1) LCD_Show_Text((alt_8*)"+");

		/* Get an int from each arg and add it to temp */
		temp = intfromstring(argv[i]);
		sum += temp;
	}
	//printf("%d\n",(int)sum);
	LCD_Line2();
	LCD_Show_Text((alt_8*)"=");
	LCD_Show_Decimal(sum);
	return 0;
}

/* Updates the red LEDs to be the binary representation of a number */
alt_32 ledr(alt_32 argc, alt_8* argv[]){
	if (argc <= 1){
		return -1;
	}
	alt_32 dec=0;
	dec = intfromstring(argv[1]);

	/* Return from the program if input wasn't a number or bigger than the max for the LEDs*/
	if ((dec == 0 && (altstrcmp(argv[1], (alt_8*)"0") != 0))
	|| dec > MAX_LED_REGISTER){
		printf("Argument 1 for ledr is invalid: %s\n", argv[1]);
		return -1;
	}
	IOWR(LED_RED_BASE, 0, dec);
	return 0;
}

/* shows the switch configuration as hex on the 7-seg display */
alt_32 switch_function(alt_32 argc, alt_8* argv[]){
	alt_32 dec = IORD(SWITCH_PIO_BASE,0);
	alt_32 i;
	alt_32* hex = calloc(8,sizeof(alt_32));
	decimaltohex(hex,dec);
	static const alt_u8  Map[] = {
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
	alt_8 path[SD_MAX_PATH_LENGTH];
	altstrcpy(path,SD_getCurrentPath());

	if (argc>1){
		SD_updatePath(path,argv[1]);
	}
	printf("PATH: %s\n",path);

	/* Read directory */
	alt_8 checkpath = ls_openDir(&list,&(efsl->myFs),path);
	//printf("checkpath=%d\n",checkpath);
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


	alt_8* path;
	path = SD_getCurrentPath();

	/* check it is a valid directory that the user wants to change the path to */
	alt_8 checkpath=0;
	if (argc>1){
		checkpath = ls_openDir(&list,&(efsl->myFs),(char*)argv[1]);
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


alt_32 make_directory(alt_32 argc, alt_8* argv[]){
	if (argc <= 1){
		printf("Usage: %s directory\n",argv[0]);
		return -1;
	}
	EmbeddedFileSystem* efsl;

	efsl = *(SD_mount());

	if (efsl==NULL)
		return -1;

	/* Get absolute path */
	alt_8 path[SD_MAX_PATH_LENGTH];
	altstrcpy(path,SD_getCurrentPath());
	SD_updatePath(path,argv[1]);

	printf("Making %s\n",path);
	alt_32 check;

	check = makedir(&(efsl->myFs), path);
	if (check != 0){
		printf("This directory already exists\n");
	}

	//SD_unmount();

	return 0;
}

/* For ability to delete directories
 * Had to edit efsl file: /FileSystemLibSrc/src/ui.c line 139 from
 * 		if((fs_findFile(fs,(eint8*)filename,&loc,0))==1){
 * to
 * 		if((fs_findFile(fs,(eint8*)filename,&loc,0))){
 */
alt_32 delete_file(alt_32 argc, alt_8* argv[]){
	EmbeddedFileSystem* efsl;

	efsl = *(SD_mount());

	if (efsl==NULL)
		return -1;

	/* Get absolute path */
	alt_8 path[SD_MAX_PATH_LENGTH];
	altstrcpy(path,SD_getCurrentPath());
	SD_updatePath(path,argv[1]);

	/* Remove the file */
	rmfile(&(efsl->myFs), (alt_u8*)path);
	return 0;
}

alt_32 write_new_file(alt_32 argc, alt_8* argv[]){
	EmbeddedFileSystem* efsl;
	File file;
	alt_8 write_buffer[SD_MAX_PATH_LENGTH];
	altmemset(write_buffer,'\0',SD_MAX_PATH_LENGTH);
	alt_32 i;
	for(i=2;i<argc;i++){
		altstrcat(write_buffer,argv[i]);
		if (i<argc-1){
			altstrcat(write_buffer," ");
		}
	}

	efsl = *(SD_mount());

	if (efsl==NULL)
		return -1;

	/* Get absolute path */
	alt_8 path[SD_MAX_PATH_LENGTH];
	altstrcpy(path,SD_getCurrentPath());
	SD_updatePath(path,argv[1]);

	if(file_fopen(&file, &(efsl->myFs), path, 'w')!=0){
				printf("Could not open file for writing\n");
				return 0;
	}
	printf("File opened for writing.\n");

	if(file_write(&file,altstrlen((alt_8*)write_buffer),(alt_u8*)write_buffer) == altstrlen((alt_8*)write_buffer)){
		printf("File written.\n");
	} else {
		printf("could not write file.\n");
	}

	file_fclose(&file);

	return 0;
}

alt_32  read_file(alt_32 argc, alt_8* argv[]){
	EmbeddedFileSystem* efsl;
	File file;
	euint8 buffer[512];
	euint16 e,f;

	efsl = *(SD_mount());

	if (efsl==NULL)
		return -1;

	/* Get absolute path */
	alt_8 path[SD_MAX_PATH_LENGTH];
	altstrcpy(path,SD_getCurrentPath());
	SD_updatePath(path,argv[1]);

	if(file_fopen(&file, &(efsl->myFs), path, 'r')!=0){
				printf("Could not open file for reading\n");
				return -1;
	}
	printf("File %s opened for reading.\n",path);

	while((e=file_read(&file,512,buffer))){
		for(f=0;f<e;f++){
			printf("%c",buffer[f]);
		}
	}

	file_fclose(&file);

	return 0;
}

alt_32 copy_file(alt_32 argc, alt_8* argv[]){
	EmbeddedFileSystem* efsl;
	File file1, file2;
	euint8 buffer[512];
	euint16 e;

	efsl = *(SD_mount());

	if (efsl==NULL)
		return -1;

	/* Get absolute path of read file */
	alt_8 read_file_path[SD_MAX_PATH_LENGTH];
	altstrcpy(read_file_path,SD_getCurrentPath());
	SD_updatePath(read_file_path,argv[1]);

	/* Get absolute path of write file */
	alt_8 write_file_path[SD_MAX_PATH_LENGTH];
	altstrcpy(write_file_path,SD_getCurrentPath());
	SD_updatePath(write_file_path,argv[2]);

	if(file_fopen(&file1, &(efsl->myFs), read_file_path, 'r')!=0){
		printf("Could not open file for reading\n");
		return -1;
	}
	printf("File opened for reading.\n");

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

	return 0;
}


#endif

